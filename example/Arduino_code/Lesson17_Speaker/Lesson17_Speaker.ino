#include <Arduino.h>     
#include <ESP_I2S.h>            // ESP32 I2S Library
#include <string.h>             // Include standard string manipulation functions
#include <esp_log.h>            // ESP-IDF logging library
#include <esp_err.h>            // ESP-IDF error codes
#include <esp_ldo_regulator.h>  // ESP32-P4 specific LDO management
#include <sys/unistd.h>         // Include system calls for file handling
#include <sys/stat.h>           // Include functions for file status and permissions
#include <esp_vfs_fat.h>        // Include ESP-IDF FAT filesystem support for SD card
#include <sdmmc_cmd.h>          // Include SDMMC card command definitions and helpers
#include <driver/sdmmc_host.h>  // Include SDMMC host driver for SD card communication

#define EXAMPLE_MAX_CHAR_SIZE 64   // Maximum character buffer size for file read/write operations
#define SD_MOUNT_POINT "/sdcard"   // Default SD card mount point path

#pragma once

/*********************** Pin definitions ***********************/
// SD card GPIO with SD_MMC interface
#define SD_GPIO_MMC_CLK     (43)
#define SD_GPIO_MMC_CMD     (44)
#define SD_GPIO_MMC_D0      (39)
// SD card GPIO with SPI interface
#define SD_GPIO_SPI_CLK     SD_GPIO_MMC_CLK
#define SD_GPIO_SPI_MOSI    SD_GPIO_MMC_CMD
#define SD_GPIO_SPI_MISO    SD_GPIO_MMC_D0

#define AUDIO_GPIO_CTRL         (6)    // GPIO pin number for audio power control
#define AUDIO_POWER_ENABLE      (LOW)   // GPIO set low level to enable audio power
#define AUDIO_POWER_DISABLE     (HIGH)  // GPIO set high level to disable audio power

#define AUDIO_GPIO_LRCLK        (21)    // GPIO pin number for I2S LRCLK (Left-Right Clock)
#define AUDIO_GPIO_BCLK         (22)    // GPIO pin number for I2S BCLK (Bit Clock)
#define AUDIO_GPIO_SDATA        (23)    // GPIO pin number for I2S SDATA (Serial Data)

#define MIC_GPIO_CLK            (3)    // GPIO pin number for microphone BCLK (Bit Clock of PDM)
#define MIC_GPIO_SDIN           (4)    // GPIO pin number for microphone SDIN (Serial Data of PDM)
/*********************** Pin definitions ***********************/

// Logging macros
#define AUDIO_ERROR(format, ...) Serial.printf("ERROR: " format "\n", ##__VA_ARGS__)
#define AUDIO_INFO(format, ...) Serial.printf("INFO: " format "\n", ##__VA_ARGS__)

static sdmmc_card_t *card;           // SD card info pointer
const char sd_mount_point[] = SD_MOUNT_POINT;

// Loudspeaker
static I2SClass i2s_spk;             // Create an I2SClass speaker instance

// WAV file header structure
typedef struct {
    char riff[4];           // "RIFF"
    uint32_t fileSize;      // File size
    char wave[4];           // "WAVE"
    char fmt[4];            // "fmt "
    uint32_t fmtSize;       // fmt chunk size
    uint16_t audioFormat;   // Audio format (1 = PCM)
    uint16_t numChannels;   // Number of channels
    uint32_t sampleRate;    // Sample rate
    uint32_t byteRate;      // Byte rate
    uint16_t blockAlign;    // Block alignment
    uint16_t bitsPerSample; // Bits per sample
    char data[4];           // "data"
    uint32_t dataSize;      // Data chunk size
} wav_header_t;

// Validate WAV file header
bool validate_wav_header(FILE *file)
{
    if (file == NULL)
    {
        AUDIO_ERROR("File pointer is NULL");
        return false;
    }
    long original_position = ftell(file); // Store current file position to restore later
    if (original_position == -1)
    {
        AUDIO_ERROR("Cannot get current file position");
        return false;
    }
    if (fseek(file, 0, SEEK_SET) != 0) // Rewind to beginning of file
    {
        AUDIO_ERROR("Cannot seek to file beginning");
        return false;
    }
    uint8_t header[44]; // Read and validate WAV header
    size_t bytes_read = fread(header, 1, 44, file);
    if (bytes_read != 44)
    {
        AUDIO_ERROR("Cannot read complete WAV header (%d bytes)", bytes_read);
        fseek(file, original_position, SEEK_SET);
        return false;
    }
    if (memcmp(header, "RIFF", 4) != 0) // Validate RIFF chunk descriptor
    {
        AUDIO_ERROR("Invalid RIFF header");
        fseek(file, original_position, SEEK_SET);
        return false;
    }
    if (memcmp(header + 8, "WAVE", 4) != 0) // Validate WAVE format
    {
        AUDIO_ERROR("Invalid WAVE format");
        fseek(file, original_position, SEEK_SET);
        return false;
    }
    if (memcmp(header + 12, "fmt ", 4) != 0) // Validate fmt subchunk
    {
        AUDIO_ERROR("Invalid fmt subchunk");
        fseek(file, original_position, SEEK_SET);
        return false;
    }
    uint16_t audio_format = *(uint16_t *)(header + 20); // Check audio format (should be 1 for PCM)
    if (audio_format != 1)
    {
        AUDIO_ERROR("Unsupported audio format: %d (only PCM supported)", audio_format);
        fseek(file, original_position, SEEK_SET);
        return false;
    }
    uint16_t num_channels = *(uint16_t *)(header + 22); // Check number of channels (support mono and stereo)
    if (num_channels != 1 && num_channels != 2)
    {
        AUDIO_ERROR("Unsupported number of channels: %d", num_channels);
        fseek(file, original_position, SEEK_SET);
        return false;
    }
    uint32_t sample_rate = *(uint32_t *)(header + 24); // Check sample rate (support common rates)
    if (sample_rate != 8000 && sample_rate != 16000 && sample_rate != 22050 && sample_rate != 44100 && sample_rate != 48000)
    {
        AUDIO_ERROR("Uncommon sample rate: %lu Hz", sample_rate);
        fseek(file, original_position, SEEK_SET);
        return false;
    }
    uint16_t bits_per_sample = *(uint16_t *)(header + 34); // Check bits per sample (support 8,16,24,32)
    if (bits_per_sample != 8 && bits_per_sample != 16 && bits_per_sample != 24 && bits_per_sample != 32)
    {   
        AUDIO_ERROR("Unsupported bits per sample: %d", bits_per_sample);
        fseek(file, original_position, SEEK_SET);
        return false;
    }
    if (memcmp(header + 36, "data", 4) != 0) // Validate data subchunk
    {
        AUDIO_ERROR("Invalid data subchunk");
        fseek(file, original_position, SEEK_SET);
        return false;
    }
    uint32_t file_size = *(uint32_t *)(header + 4) + 8; // RIFF block size + 8-byte header
    uint32_t data_size = *(uint32_t *)(header + 40);

    AUDIO_INFO("WAV File Info: %d channels, %lu Hz, %d bits, %lu bytes data, %lu bytes total",
                num_channels, sample_rate, bits_per_sample, data_size, file_size);
    fseek(file, original_position, SEEK_SET); // Restore original file position
    return true;
}

// Control audio hardware power
static void set_Audio_ctrl(bool state) {
    // Hardware control: enable or disable audio power
    bool status = !state;
    digitalWrite(AUDIO_GPIO_CTRL, status);
    if (state) {
        AUDIO_INFO("Audio hardware enabled");
    } else {
        AUDIO_INFO("Audio hardware disabled");
    }
}

// Play WAV file from SD card
esp_err_t Audio_play_wav_sd(const char *filename)
{
    esp_err_t err = ESP_OK;
    if (filename == NULL)
        return ESP_ERR_INVALID_ARG;

    FILE *fh = fopen(filename, "rb");
    if (fh == NULL)
    {
        Serial.printf("Failed to open file\n");
        return ESP_ERR_INVALID_ARG;
    }

    if (!validate_wav_header(fh)) // Validate WAV header
    {
        Serial.printf("Invalid WAV file format: %s\n", filename);
        fclose(fh);
        return ESP_ERR_INVALID_ARG;
    }

    if (fseek(fh, 44 + 4000, SEEK_SET) != 0) // Skip 44-byte header + 4000 bytes of data
    {
        Serial.printf("Failed to seek file\n");
        fclose(fh);
        return ESP_FAIL;
    }

    const size_t SAMPLES_PER_BUFFER = 512; // Number of samples per buffer
    const size_t INPUT_BUFFER_SIZE = SAMPLES_PER_BUFFER * sizeof(int16_t);
    const size_t OUTPUT_BUFFER_SIZE = SAMPLES_PER_BUFFER * 2 * sizeof(int16_t);

    int16_t *input_buf = (int16_t*)heap_caps_malloc(INPUT_BUFFER_SIZE, MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA | MALLOC_CAP_32BIT);
    int16_t *output_buf = (int16_t*)heap_caps_malloc(OUTPUT_BUFFER_SIZE, MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA | MALLOC_CAP_32BIT);

    if (input_buf == NULL || output_buf == NULL)
    {
        Serial.printf("Failed to allocate audio buffers\n");
        if (input_buf) free(input_buf);
        if (output_buf) free(output_buf);
        fclose(fh);
        return ESP_ERR_NO_MEM;
    }

    size_t samples_read = 0;
    size_t bytes_to_write = 0;
    size_t bytes_written = 0;
    size_t total_samples = 0;
    int32_t volume_data = 0;

    digitalWrite(AUDIO_GPIO_CTRL, AUDIO_POWER_ENABLE); // Enable audio power

    while (1)
    {
        samples_read = fread(input_buf, sizeof(int16_t), SAMPLES_PER_BUFFER, fh);
        if (samples_read == 0)
            break;

        for (size_t i = 0; i < samples_read; i++) // Convert mono to stereo
        {
            volume_data = input_buf[i] * 2; // Linear volume multiplication
            if (volume_data > 32767) volume_data = 32767;
            else if (volume_data < -32768) volume_data = -32768;
            output_buf[i] = (int16_t)volume_data; // Left channel
        }

        bytes_to_write = samples_read * sizeof(int16_t);
        bytes_written = i2s_spk.write((uint8_t*)output_buf, bytes_to_write); // Write to I2S
        if (bytes_written != bytes_to_write)
        {
            Serial.printf("I2S write failed: %s, written: %d/%d\n", esp_err_to_name(err), bytes_written, bytes_to_write);
            break;
        }
        total_samples += samples_read;
    }

    digitalWrite(AUDIO_GPIO_CTRL, AUDIO_POWER_DISABLE); // Disable audio power
    free(input_buf);
    free(output_buf);
    fclose(fh);
    Serial.printf("Audio playback completed: %d samples\n", total_samples);
    return err;
}

// Initialize ESP32-P4 LDOs
esp_err_t board_p4_ldo_init() 
{
    esp_err_t err = ESP_OK;
    esp_ldo_channel_handle_t ldo3_handle = NULL;
    esp_ldo_channel_config_t ldo3_cfg = {
        .chan_id = 3,           // LDO3 channel
        .voltage_mv = 2500,     // 2.5V
    };

    Serial.println("Initializing LDO3 to 2.5V...");
    err = esp_ldo_acquire_channel(&ldo3_cfg, &ldo3_handle);
    if (err != ESP_OK) { Serial.printf("LDO3 Power Error: %s\n", esp_err_to_name(err)); return err; }

    esp_ldo_channel_handle_t ldo4_handle = NULL;
    esp_ldo_channel_config_t ldo4_cfg = { .chan_id = 4, .voltage_mv = 3300 }; // LDO4: 3.3V

    Serial.println("Initializing LDO4 to 3.3V...");
    err = esp_ldo_acquire_channel(&ldo4_cfg, &ldo4_handle);
    if (err != ESP_OK) { Serial.printf("LDO4 Power Error: %s\n", esp_err_to_name(err)); return err; }

    return ESP_OK;
}

// Initialize SD card (SD_MMC mode)
esp_err_t sd_init()
{
    esp_err_t err = ESP_OK;
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024,
    };

    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    host.slot = SDMMC_HOST_SLOT_0;
    host.max_freq_khz = 10000;

    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.clk = (gpio_num_t)SD_GPIO_MMC_CLK;
    slot_config.cmd = (gpio_num_t)SD_GPIO_MMC_CMD;
    slot_config.d0 = (gpio_num_t)SD_GPIO_MMC_D0;
    slot_config.width = 1; // 1-bit SDIO
    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;

    err = esp_vfs_fat_sdmmc_mount(sd_mount_point, &host, &slot_config, &mount_config, &card);
    if (err != ESP_OK) {
        Serial.printf("Failed to initialize SD card (%s)\n", esp_err_to_name(err));
        return err;
    }
    Serial.println("SD card mounted successfully");
    sdmmc_card_print_info(stdout, card);
    return err;
}

// Print SD card information
void get_sd_card_info()
{
    sdmmc_card_print_info(stdout, card);
}

// Initialize I2S speaker
void speaker_init()
{
    i2s_spk.setPins(AUDIO_GPIO_BCLK, AUDIO_GPIO_LRCLK, AUDIO_GPIO_SDATA); // BCLK, LRCLK, DOUT
    if (!i2s_spk.begin(I2S_MODE_STD, 16000, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO, I2S_STD_SLOT_BOTH)) {
        Serial.println("I2S output mode initialization failed!");
        while (1) delay(1000);
    }
    Serial.println("I2S speaker initialized");
}

// Arduino setup
void setup()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("ESP32-P4 Audio Player Starting...");

    board_p4_ldo_init(); // Initialize LDOs
    pinMode(AUDIO_GPIO_CTRL, OUTPUT);
    digitalWrite(AUDIO_GPIO_CTRL, AUDIO_POWER_DISABLE); // Power off audio initially
    
    if (sd_init() != ESP_OK) {
        Serial.println("SD card initialization failed!");
        return;
    }
    
    speaker_init(); // Initialize speaker
    if (Audio_play_wav_sd("/sdcard/test.wav") != ESP_OK) { // Play WAV file
        Serial.println("Audio playback failed!");
    }
}

// Arduino main loop
void loop()
{
    delay(1000);
}