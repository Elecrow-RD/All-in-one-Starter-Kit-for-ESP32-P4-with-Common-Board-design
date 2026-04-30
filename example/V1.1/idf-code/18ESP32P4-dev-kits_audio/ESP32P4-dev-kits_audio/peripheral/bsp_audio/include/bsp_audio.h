#ifndef _BSP_AUDIO_H_
#define _BSP_AUDIO_H_

/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "esp_log.h"        //References for LOG Printing Function-related API Functions
#include "esp_err.h"        //References for Error Type Function-related API Functions
#include "driver/gpio.h"    //References for GPIO Function-related API Functions
#include "driver/i2s_std.h" //References for I2S STD Function-related API Functions
#ifdef CONFIG_BSP_SD_ENABLED
#include "bsp_sd.h"
#endif
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#define AUDIO_TAG "AUDIO"
#define AUDIO_INFO(fmt, ...) ESP_LOGI(AUDIO_TAG, fmt, ##__VA_ARGS__)
#define AUDIO_DEBUG(fmt, ...) ESP_LOGD(AUDIO_TAG, fmt, ##__VA_ARGS__)
#define AUDIO_ERROR(fmt, ...) ESP_LOGE(AUDIO_TAG, fmt, ##__VA_ARGS__)

#ifdef CONFIG_BSP_AUDIO_ENABLED

#define AUDIO_GPIO_LRCLK CONFIG_AUDIO_GPIO_LRCLK // Audio LRCLK GPIO
#define AUDIO_GPIO_BCLK CONFIG_AUDIO_GPIO_BCLK   // Audio BCLK GPIO
#define AUDIO_GPIO_SDATA CONFIG_AUDIO_GPIO_SDATA // Audio DATA GPIO
#define AUDIO_GPIO_CTRL CONFIG_AUDIO_GPIO_CTRL   // Audio CTRL GPIO

esp_err_t audio_init();               // Audio Initialization Function
esp_err_t audio_ctrl_init();          // Audio ctrl Initialization Function
esp_err_t set_Audio_ctrl(bool state); // Control mute
#ifdef CONFIG_BSP_SD_ENABLED
esp_err_t Audio_play_wav_sd(const char *fp); // The speaker plays the wav file stored on the SD card
#endif
#endif
/*———————————————————————————————————————Variable declaration end——————————————-—————————————————————————*/
#endif