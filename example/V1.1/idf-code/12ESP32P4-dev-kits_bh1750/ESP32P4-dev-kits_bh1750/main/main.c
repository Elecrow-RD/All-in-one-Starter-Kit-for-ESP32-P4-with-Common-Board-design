/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "main.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#ifdef CONFIG_BSP_BH1750_SENSOR_ENABLED
TaskHandle_t read_bh1750;
#ifdef CONFIG_BSP_DISPLAY_ENABLED
static lv_obj_t *bh1750_data = NULL;
#endif
#endif
static esp_ldo_channel_handle_t ldo4 = NULL;
static esp_ldo_channel_handle_t ldo3 = NULL;
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_BH1750_SENSOR_ENABLED
#ifdef CONFIG_BSP_DISPLAY_ENABLED
void bh1750_display()
{
    if (lvgl_port_lock(0))
    {
        bh1750_data = lv_label_create(lv_scr_act()); /*Create a label object*/
        static lv_style_t label_style;
        lv_style_init(&label_style);                                                   /*Initialize a style*/
        lv_style_set_bg_opa(&label_style, LV_OPA_TRANSP);                              /*Set the style LVGL background color*/
        lv_obj_add_style(bh1750_data, &label_style, LV_PART_MAIN);                     /*Add a style to an object*/
        lv_obj_set_style_text_color(bh1750_data, LV_COLOR_WHITE, LV_PART_MAIN);        /*Set the style LVGL text color*/
        lv_obj_set_style_text_font(bh1750_data, &lv_font_montserrat_30, LV_PART_MAIN); /*Set the style LVGL text font*/
        lv_obj_center(bh1750_data);                                                    /*Align an object to the center on its parent*/
        lv_obj_set_style_bg_color(lv_scr_act(), LV_COLOR_BLACK, LV_PART_MAIN);         /*Set the screen's LVGL background color*/
        lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, LV_PART_MAIN);             /*Set the screen's LVGL background transparency*/
        lv_label_set_text(bh1750_data, "lux = 0.0");                                   /*Set a new text for a label*/
        lvgl_port_unlock();
    }
}

void update_bh1750_value(float lux)
{
    if (bh1750_data)
    {
        char buffer[40];
        snprintf(buffer, sizeof(buffer), "lux = %.1f", lux); /*Format the data into a string*/
        lv_label_set_text(bh1750_data, buffer);              /*Set a new text for a label*/
    }
}

void bh1750_read_task(void *param)
{
    float lux = 0;
    while (1)
    {
        lux = bh1750_read_data(); /*Read the illuminance data of the BH1750 sensor*/
        if (lux == (-1))
        {
            if (lvgl_port_lock(0))
            {
                lv_label_set_text(bh1750_data, "bh1750 read data error"); /*Read failure message displayed*/
                lvgl_port_unlock();
            }
        }
        else
        {
            MAIN_INFO("lux:%.2f", lux);
            if (lvgl_port_lock(0))
            {
                update_bh1750_value(lux); /*Update the Bh1750 data displayed on the screen*/
                lvgl_port_unlock();
            }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
#endif
#endif

void init_fail(const char *name, esp_err_t err)
{
    static bool state = false;
    while (1)
    {
        if (!state)
        {
            MAIN_ERROR("%s init  [ %s ]", name, esp_err_to_name(err));
            state = true;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void Init(void)
{
    static esp_err_t err = ESP_OK;
    esp_ldo_channel_config_t ldo3_cof = {
        .chan_id = 3,
        .voltage_mv = 2500,
    };
    err = esp_ldo_acquire_channel(&ldo3_cof, &ldo3);
    if (err != ESP_OK)
        init_fail("ldo3", err);
    esp_ldo_channel_config_t ldo4_cof = {
        .chan_id = 4,
        .voltage_mv = 3300,
    };
    err = esp_ldo_acquire_channel(&ldo4_cof, &ldo4);
    if (err != ESP_OK)
        init_fail("ldo4", err);
#ifdef CONFIG_BSP_I2C_ENABLED
    err = i2c_init(); /*I2C Initialization*/
    if (err != ESP_OK)
        init_fail("i2c", err);
    vTaskDelay(200 / portTICK_PERIOD_MS);
#ifdef CONFIG_BSP_BH1750_SENSOR_ENABLED
    err = bh1750_begin(); /*BH1750 Initialization*/
    if (err != ESP_OK)
        init_fail("bh1750", err);
#endif
#endif
#ifdef CONFIG_BSP_DISPLAY_ENABLED
    err = display_init(); /*Display Initialization*/
    if (err != ESP_OK)
        init_fail("display", err);
#endif
}

void app_main(void)
{
    MAIN_INFO("----------Demo version----------");
    MAIN_INFO("----------Start the test----------");
    Init();
#ifdef CONFIG_BSP_BH1750_SENSOR_ENABLED
#ifdef CONFIG_BSP_DISPLAY_ENABLED
    set_lcd_blight(100);                                                                              /*Set the screen backlight to maximum brightness*/
    bh1750_display();                                                                                 /*Set the screen's LVGL default display page*/
    xTaskCreate(bh1750_read_task, "read_bh1750", 4096, NULL, configMAX_PRIORITIES - 5, &read_bh1750); /*Create a BH1750 data display refresh thread*/
#endif
#endif
}
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/