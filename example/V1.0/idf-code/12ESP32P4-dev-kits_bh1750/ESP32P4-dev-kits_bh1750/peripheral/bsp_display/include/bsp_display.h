#ifndef _BSP_DISPLAY_H_
#define _BSP_DISPLAY_H_
/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "esp_log.h"           //References for LOG Printing Function-related API Functions
#include "esp_err.h"           //References for Error Type Function-related API Functions
#include "esp_ldo_regulator.h" //References for LDO Function-related API Functions
#include "esp_lcd_ek79007.h"   //References for lcd ek79007 Function-related API Functions
#include "esp_lcd_mipi_dsi.h"  //References for lcd mipi dsi Function-related API Functions
#include "esp_lcd_panel_ops.h" //References for lcd panel ops Function-related API Functions
#include "esp_lcd_panel_io.h"  //References for lcd panel io Function-related API Functions
#include "esp_lvgl_port.h"     //References for LVGL port Function-related API Functions
#include "driver/gpio.h"       //References for GPIO Function-related API Functions
#include "driver/ledc.h"       //References for LEDC PWM Function-related API Functions
#include "lvgl.h"              //References for LVGL Function-related API Functions
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#define DISPLAY_TAG "DISPLAY"
#define DISPLAY_INFO(fmt, ...) ESP_LOGI(DISPLAY_TAG, fmt, ##__VA_ARGS__)
#define DISPLAY_DEBUG(fmt, ...) ESP_LOGD(DISPLAY_TAG, fmt, ##__VA_ARGS__)
#define DISPLAY_ERROR(fmt, ...) ESP_LOGE(DISPLAY_TAG, fmt, ##__VA_ARGS__)

#ifdef CONFIG_BSP_DISPLAY_ENABLED

#define V_size CONFIG_V_SIZE                 // Vertical resolution
#define H_size CONFIG_H_SIZE                 // Horizontal resolution
#define BITS_PER_PIXEL CONFIG_BITS_PER_PIXEL // Number of image display bits of the display screen

#define LCD_GPIO_RESET CONFIG_LCD_GPIO_RESET   // LCD Reset GPIO
#define LCD_GPIO_BLIGHT CONFIG_LCD_GPIO_BLIGHT // LCD Blight GPIO
#define LCD_GPIO_UPDN CONFIG_LCD_GPIO_UPDN     // LCD UPDN GPIO
#define LCD_GPIO_SHLR CONFIG_LCD_GPIO_SHLR     // LCD SHLR GPIO
#define BLIGHT_PWM_Hz CONFIG_BLIGHT_PWM_Hz     // LCD Blight PWM GPIO

#define LV_COLOR_RED lv_color_make(0xFF, 0x00, 0x00)    // LVGL Red
#define LV_COLOR_GREEN lv_color_make(0x00, 0xFF, 0x00)  // LVGL Green
#define LV_COLOR_BLUE lv_color_make(0x00, 0x00, 0xFF)   // LVGL Blue
#define LV_COLOR_WHITE lv_color_make(0xFF, 0xFF, 0xFF)  // LVGL White
#define LV_COLOR_BLACK lv_color_make(0x00, 0x00, 0x00)  // LVGL Black
#define LV_COLOR_GRAY lv_color_make(0x80, 0x80, 0x80)   // LVGL gray
#define LV_COLOR_YELLOW lv_color_make(0xFF, 0xFF, 0x00) // LVGL yellow

esp_err_t display_init();                      // Display Screen Initialization Function
esp_err_t set_lcd_blight(uint32_t brightness); // Set the screen backlight
#endif
/*———————————————————————————————————————Variable declaration end——————————————-—————————————————————————*/
#endif
