/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "bsp_mic.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#ifdef CONFIG_BSP_MIC_ENABLED
static i2s_chan_handle_t rx_chan;
int16_t i2s_readraw_buff[16000];
#endif
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_MIC_ENABLED
esp_err_t mic_init()
{
    esp_err_t err = ESP_OK;
    /*I2S channel configuration for receiver (microphone)*/
    i2s_chan_config_t rx_chan_cfg = {
        .id = I2S_NUM_0,               /*Use I2S controller 0*/
        .role = I2S_ROLE_MASTER,       /*I2S acts as master (generates clock signals)*/
        .dma_desc_num = 6,             /*Number of DMA descriptors (affects buffer management)*/
        .dma_frame_num = 256,          /*Number of frames per DMA descriptor*/
        .auto_clear_after_cb = false,  /*Don't auto-clear DMA buffer after callback*/
        .auto_clear_before_cb = false, /*Don't auto-clear DMA buffer before callback*/
        .allow_pd = false,             /*Don't allow power down during operation*/
        .intr_priority = 0,            /*Interrupt priority (0 = default)*/
    };
    /*Create new I2S channel (receive channel only, no transmit channel)*/
    err = i2s_new_channel(&rx_chan_cfg, NULL, &rx_chan);
    if (err != ESP_OK)
        return err;
    /*PDM (Pulse Density Modulation) receiver configuration*/
    i2s_pdm_rx_config_t pdm_rx_cfg = {
        /*Clock configuration*/
        .clk_cfg = {
            .sample_rate_hz = MIC_SAMPLE_RATE,      /*Sampling frequency (e.g., 16kHz, 44.1kHz)*/
            .clk_src = I2S_CLK_SRC_DEFAULT,         /*Default clock source (usually APLL or PLL)*/
            .mclk_multiple = I2S_MCLK_MULTIPLE_256, /*Master clock multiple (256 × sample rate)*/
            .dn_sample_mode = I2S_PDM_DSR_8S,       /*Downsampling mode (8s for PDM to PCM conversion)*/
            .bclk_div = 8,                          /*Bit clock divider*/
        },
        /*Slot configuration (data format)*/
        .slot_cfg = {
            .data_bit_width = I2S_DATA_BIT_WIDTH_16BIT, /*Data bit width (16-bit for PDM)*/
            .slot_bit_width = I2S_SLOT_BIT_WIDTH_AUTO,  /*Auto-calculate slot bit width*/
            .slot_mode = I2S_SLOT_MODE_MONO,            /*Mono audio (single channel)*/
            .slot_mask = I2S_PDM_SLOT_LEFT,             /*Use left slot for mono data*/
            .hp_en = true,                              /*Enable high-pass filter*/
            .hp_cut_off_freq_hz = 35.5,                 /*High-pass filter cutoff frequency (35.5Hz)*/
            .amplify_num = 1,                           /*Amplification factor (1 = no amplification)*/
        },
        /*GPIO pin configuration*/
        .gpio_cfg = {
            .clk = MIC_GPIO_CLK,  /*GPIO pin for PDM clock signal*/
            .din = MIC_GPIO_DATA, /*GPIO pin for PDM data input*/
            .invert_flags = {
                .clk_inv = false, /*Don't invert clock polarity*/
            },
        },
    };
    err = i2s_channel_init_pdm_rx_mode(rx_chan, &pdm_rx_cfg); /*Initialize I2S channel in PDM receiver mode*/
    if (err != ESP_OK)
        return err;
    err = i2s_channel_enable(rx_chan); /*Enable the I2S channel to start receiving audio data*/
    if (err != ESP_OK)
        return err;
    return err;
}

static void generate_wav_header(char *wav_header, uint32_t wav_size, uint32_t sample_rate)
{
    uint32_t file_size = wav_size + 44 - 8;
    uint32_t byte_rate = BYTE_RATE;
    const char set_wav_header[] = {
        'R', 'I', 'F', 'F',                                                  /*ChunkID*/
        file_size, file_size >> 8, file_size >> 16, file_size >> 24,         /*ChunkSize*/
        'W', 'A', 'V', 'E',                                                  /*Format*/
        'f', 'm', 't', ' ',                                                  /*Subchunk1ID*/
        0x10, 0x00, 0x00, 0x00,                                              /*Subchunk1Size (16 for PCM)*/
        0x01, 0x00,                                                          /*AudioFormat (1 for PCM)*/
        0x01, 0x00,                                                          /*NumChannels (1 channel)*/
        sample_rate, sample_rate >> 8, sample_rate >> 16, sample_rate >> 24, /*SampleRate*/
        byte_rate, byte_rate >> 8, byte_rate >> 16, byte_rate >> 24,         /*ByteRate*/
        0x02, 0x00,                                                          /*BlockAlign*/
        0x10, 0x00,                                                          /*BitsPerSample (16 bits)*/
        'd', 'a', 't', 'a',                                                  /*Subchunk2ID*/
        wav_size, wav_size >> 8, wav_size >> 16, wav_size >> 24,             /*Subchunk2Size*/
    };
    memcpy(wav_header, set_wav_header, sizeof(set_wav_header));
}

#ifdef CONFIG_BSP_SD_ENABLED
esp_err_t mic_readwav_to_sd(const char *filename, size_t rec_seconds, size_t *out_size)
{
    esp_err_t err = ESP_OK;
    size_t flash_wr_size = 0; /*Total bytes written to SD card (audio data only)*/
    size_t bytes_read = 0;    /*Bytes read from I2S in each iteration*/
    if (rec_seconds > 3600 || filename == NULL || out_size == NULL)
    {
        MIC_INFO("Exceeding the maximum recording duration");
        return ESP_ERR_INVALID_ARG;
    }
    if (sizeof(i2s_readraw_buff) < 16000)
    {
        MIC_INFO("I2S buffer too small");
        return ESP_ERR_INVALID_SIZE;
    }
    size_t rec_size = rec_seconds * BYTE_RATE; /*Calculate total audio data size (excluding WAV header)*/
    char wav_header[44];
    generate_wav_header(wav_header, rec_size, MIC_SAMPLE_RATE); /*Generate WAV file header (44 bytes standard WAV header)*/
    err = write_file(filename, wav_header, 44);                 /*Write WAV header to file (creates file and writes header)*/
    if (err != ESP_OK)
        return err;
    MIC_INFO("Start Recording %d of audio data", rec_seconds);
    while (flash_wr_size < rec_size) /*Record audio data in chunks and append to file*/
    {
        size_t bytes_to_read = MIN(16000, rec_size - flash_wr_size);
        err = i2s_channel_read(rx_chan, i2s_readraw_buff, bytes_to_read, &bytes_read, portMAX_DELAY); /*Read 16000 bytes (16KB) of raw audio data from I2S microphone*/
        if (err != ESP_OK)
        {
            MIC_INFO("read mic data fail");
            return err;
        }
        err = write_file_seek(filename, i2s_readraw_buff, bytes_read, flash_wr_size + 44); /*Write audio data chunk to file at correct position (after 44-byte header)*/
        if (err != ESP_OK)
        {
            MIC_INFO("write sd of audio data fail");
            return err;
        }
        flash_wr_size += bytes_read; /*Update progress counter*/
    }
    *out_size = rec_size + 44; /*Set output parameter to total file size (audio data + WAV header)*/
    return ESP_OK;
}
#endif
#endif
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/