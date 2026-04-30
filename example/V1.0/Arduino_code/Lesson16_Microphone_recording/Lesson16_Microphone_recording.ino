#include <Arduino.h>
#include <FS.h>
#include <SD_MMC.h>          // Depending on SD card type, SD.h can also be used
#include <ESP_I2S.h>         // Provides I2SClass definition
#include <esp_ldo_regulator.h>  // ESP32-P4 specific LDO management
#include <sys/unistd.h>         // Include system calls for file handling
#include <sys/stat.h>           // Include functions for file status and permissions
#include <esp_vfs_fat.h>        // Include ESP-IDF FAT filesystem support for SD card
#include <sdmmc_cmd.h>          // Include SDMMC card command definitions and helpers
#include <driver/sdmmc_host.h>  // Include SDMMC host driver for SD card communication

// ========== Pin definitions (modify according to your wiring) ==========

#pragma once

#define AUDIO_GPIO_CTRL         (30)    // GPIO pin number for audio power
#define AUDIO_POWER_ENABLE      (LOW)   // GPIO set low level to enable audio power
#define AUDIO_POWER_DISABLE     (HIGH)  // GPIO set high level to disable audio power

#define MIC_GPIO_CLK            (3)    // GPIO pin number for microphone BCLK (Bit Clock of PDM)
#define MIC_GPIO_SDIN           (4)    // GPIO pin number for microphone SDIN (Serial Data of PDM)

// SD card GPIO with SD_MMC
#define SD_GPIO_MMC_CLK     (43)
#define SD_GPIO_MMC_CMD     (44)
#define SD_GPIO_MMC_D0      (39)
// SD card GPIO with SPI
#define SD_GPIO_SPI_CLK     SD_GPIO_MMC_CLK
#define SD_GPIO_SPI_MOSI    SD_GPIO_MMC_CMD
#define SD_GPIO_SPI_MISO    SD_GPIO_MMC_D0

// ========== Recording parameters ==========
#define SAMPLE_RATE      16000        // Sample rate 16kHz
#define RECORD_SECONDS   5            // Recording duration (seconds)
#define BYTE_RATE        (SAMPLE_RATE * 16 / 8) // Bytes per second for 16-bit mono
#define BUFFER_SIZE      16000        // Buffer size for each read (bytes)

// ========== Global I2S object ==========
static I2SClass i2s_mic;             // I2S object for microphone input

// ========== Buffer ==========
uint8_t i2s_readraw_buff[BUFFER_SIZE];  // Temporary buffer to store I2S data

#define EXAMPLE_MAX_CHAR_SIZE 64   // Maximum character buffer size for file read/write operations
#define SD_MOUNT_POINT "/sdcard"   // Default SD card mount point path

static sdmmc_card_t *card;           // Pointer to SD card information
const char sd_mount_point[] = SD_MOUNT_POINT;

// Initialize ESP32-P4 LDO power rails for audio and peripheral devices
esp_err_t board_p4_ldo_init() 
{
    esp_err_t err = ESP_OK;
    esp_ldo_channel_handle_t ldo3_handle = NULL;
    esp_ldo_channel_config_t ldo3_cfg = {
        .chan_id = 3,           // LDO Channel 3
        .voltage_mv = 2500,     // Set to 2500mV (2.5V)
    };

    Serial.println("Initializing LDO3 to 2.5V...");
    err = esp_ldo_acquire_channel(&ldo3_cfg, &ldo3_handle); // Acquire LDO3 channel
    if (err != ESP_OK) {
        Serial.printf("LDO3 Power Error: %s\n", esp_err_to_name(err));
        return err;
    } else {
        Serial.println("LDO3 Power enabled successfully.");
    }
    
    // --- Power Configuration (LDO4 for I2C/touch pull up) ---
    esp_ldo_channel_handle_t ldo4_handle = NULL;
    esp_ldo_channel_config_t ldo4_cfg = {
        .chan_id = 4,           // LDO Channel 4
        .voltage_mv = 3300,     // Set to 3300mV (3.3V)
    };

    Serial.println("Initializing LDO4 to 3.3V...");
    err = esp_ldo_acquire_channel(&ldo4_cfg, &ldo4_handle); // Acquire LDO4 channel
    if (err != ESP_OK) {
        Serial.printf("LDO4 Power Error: %s\n", esp_err_to_name(err));
        return err;
    } else {
        Serial.println("LDO4 Power enabled successfully.");
    }

    return ESP_OK;
}

// ========== Generate WAV header ==========
void generate_wav_header(uint8_t *header, size_t data_size, uint32_t sample_rate) {
    // RIFF chunk descriptor
    memcpy(header, "RIFF", 4);
    uint32_t chunk_size = data_size + 36;
    memcpy(header + 4, &chunk_size, 4);
    memcpy(header + 8, "WAVE", 4);

    // fmt sub-chunk
    memcpy(header + 12, "fmt ", 4);
    uint32_t fmt_size = 16;
    memcpy(header + 16, &fmt_size, 4);
    uint16_t audio_format = 1;           // PCM
    memcpy(header + 20, &audio_format, 2);
    uint16_t num_channels = 1;           // Mono
    memcpy(header + 22, &num_channels, 2);
    memcpy(header + 24, &sample_rate, 4);
    uint32_t byte_rate = sample_rate * num_channels * 16 / 8;
    memcpy(header + 28, &byte_rate, 4);
    uint16_t block_align = num_channels * 16 / 8;
    memcpy(header + 32, &block_align, 2);
    uint16_t bits_per_sample = 16;       // 16-bit samples
    memcpy(header + 34, &bits_per_sample, 2);

    // data sub-chunk
    memcpy(header + 36, "data", 4);
    memcpy(header + 40, &data_size, 4);
}

// ========== Initialize I2S microphone (PDM mode) ==========
bool init_mic() {
    // Set GPIO pins for PDM receive mode
    i2s_mic.setPinsPdmRx(MIC_GPIO_CLK, MIC_GPIO_SDIN);
    
    // Begin I2S in PDM receive mode with 16kHz, 16-bit mono
    if (!i2s_mic.begin(I2S_MODE_PDM_RX, 16000, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO, I2S_STD_SLOT_LEFT)) {
        Serial.println("PDM input initialization failed!");
        while (1) delay(1000);  // Halt execution on failure
    }
    
    return true;
}

// ========== Record audio and save as WAV to SD card ==========
int mic_read_to_sd(const char *filename, size_t rec_seconds, size_t *out_size) {
    if (rec_seconds > 3600 || filename == NULL) {
        return 1; // Invalid parameter
    }
    if (BUFFER_SIZE < 16000) {
        return 1; // Buffer too small
    }

    size_t rec_size = rec_seconds * BYTE_RATE;          // Total audio data size
    uint8_t wav_header[44];
    generate_wav_header(wav_header, rec_size, SAMPLE_RATE);

    // Open SD card file in binary write mode
    FILE *file = fopen(filename, "wb");
    if (!file) {
        Serial.println("Failed to open file for writing");
        return 2;
    }

    // Write WAV header
    if (fwrite(wav_header, 1, 44, file) != 44) {
        fclose(file);
        return 3;
    }

    size_t written_total = 0;
    while (written_total < rec_size) {
        size_t bytes_to_read = min((size_t)BUFFER_SIZE, rec_size - written_total);
        size_t bytes_read = 0;

        // Read bytes from I2S microphone
        bytes_read = i2s_mic.readBytes((char*)i2s_readraw_buff, bytes_to_read);
        if (bytes_read == 0) {
            Serial.println("I2S read failed");
            fclose(file);
            return 4;
        }

        // Seek to correct position in file (after WAV header)
        if (fseek(file, written_total + 44, SEEK_SET) != 0) {
            fclose(file);
            return 5;
        }

        // Write buffer data to file
        if (fwrite(i2s_readraw_buff, 1, bytes_read, file) != bytes_read) {
            fclose(file);
            return 6;
        }

        written_total += bytes_read; // Increment total written bytes
    }

    fclose(file);
    if (out_size != NULL) {
        *out_size = rec_size + 44;  // Return total file size including header
    }
    return 0;
}

// ========== Initialize SD card (SD_MMC mode) ==========
esp_err_t sd_init()
{
    esp_err_t err = ESP_OK;
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        // If SD card file system is not FAT32, mount fails unless format_if_mount_failed=true
        .format_if_mount_failed = false,    
        .max_files = 5,
        .allocation_unit_size = 16 * 1024,
    };

    sdmmc_host_t host =  SDMMC_HOST_DEFAULT(); // Use default SDMMC host
    host.slot = SDMMC_HOST_SLOT_0;
    host.max_freq_khz = 10000;                // Limit clock frequency

    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT(); // Default slot config
    slot_config.clk = (gpio_num_t)SD_GPIO_MMC_CLK;
    slot_config.cmd = (gpio_num_t)SD_GPIO_MMC_CMD;
    slot_config.d0 = (gpio_num_t)SD_GPIO_MMC_D0;
    slot_config.width = 1;  // Use 1-line SDIO
    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP; // Enable internal pull-ups

    // Mount SD card filesystem
    err = esp_vfs_fat_sdmmc_mount(sd_mount_point, &host, &slot_config, &mount_config, &card);
    if (err != ESP_OK) {
        if (err == ESP_FAIL) {
            Serial.println("Failed to mount filesystem.");
        } else {
            Serial.printf("Failed to initialize the card (%s).\n", esp_err_to_name(err));
        }
        return err;
    }
    Serial.println("SD card mounted successfully");
    sdmmc_card_print_info(stdout, card); // Print SD card info
    return err;
}

// ========== Arduino main program ==========
void setup() {
    Serial.begin(115200);               // Initialize Serial for debugging
    board_p4_ldo_init();                // Initialize LDO power rails
    // Initialize SD card (using SD_MMC mode, 1-line data)
    if (sd_init() != ESP_OK) {
        Serial.println("SD card initialization failed!");
        return;
    }
    
    // Initialize microphone
    if (!init_mic()) {
        Serial.println("Mic initialization failed");
        return;
    }
    
    // Start recording 5 seconds
    Serial.println("Start recording...");
    size_t total_size;
    int ret = mic_read_to_sd("/sdcard/test.wav", RECORD_SECONDS, &total_size);
    if (ret == 0) {
        Serial.printf("Recording finished, file size: %u bytes\n", total_size);
    } else {
        Serial.printf("Recording failed, error code: %d\n", ret);
    }
    
    // Stop I2S interface
    i2s_mic.end();
}

void loop() {
    // Empty loop, user can add other tasks here
}