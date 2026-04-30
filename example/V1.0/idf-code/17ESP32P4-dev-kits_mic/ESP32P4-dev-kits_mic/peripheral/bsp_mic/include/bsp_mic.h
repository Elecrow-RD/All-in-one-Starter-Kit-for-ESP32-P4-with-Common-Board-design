#ifndef _BSP_MIC_H_
#define _BSP_MIC_H_

/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "esp_log.h"        //References for LOG Printing Function-related API Functions
#include "esp_err.h"        //References for Error Type Function-related API Functions
#include "driver/i2s_pdm.h" //References for I2S PDM Function-related API Functions
#ifdef CONFIG_BSP_SD_ENABLED
#include "bsp_sd.h"
#endif
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#define MIC_TAG "MIC"
#define MIC_INFO(fmt, ...) ESP_LOGI(MIC_TAG, fmt, ##__VA_ARGS__)
#define MIC_DEBUG(fmt, ...) ESP_LOGD(MIC_TAG, fmt, ##__VA_ARGS__)
#define MIC_ERROR(fmt, ...) ESP_LOGE(MIC_TAG, fmt, ##__VA_ARGS__)

#ifdef CONFIG_BSP_MIC_ENABLED

#define MIC_GPIO_CLK CONFIG_MIC_GPIO_CLK           // Microphone CLK GPIO
#define MIC_GPIO_DATA CONFIG_MIC_GPIO_DATA         // Microphone DATA GPIO
#define MIC_SAMPLE_RATE CONFIG_MIC_SAMPLE_RATE     // Microphone recording sampling rate
#define BYTE_RATE (MIC_SAMPLE_RATE * (16 / 8)) * 1 // The data volume recorded by the microphone in one second

esp_err_t mic_init(); // Microphone Initialization Function
#ifdef CONFIG_BSP_SD_ENABLED
esp_err_t mic_readwav_to_sd(const char *filename, size_t rec_seconds, size_t *out_size); // Microphone recording saved to SD card
#endif
#endif
/*———————————————————————————————————————Variable declaration end——————————————-—————————————————————————*/
#endif