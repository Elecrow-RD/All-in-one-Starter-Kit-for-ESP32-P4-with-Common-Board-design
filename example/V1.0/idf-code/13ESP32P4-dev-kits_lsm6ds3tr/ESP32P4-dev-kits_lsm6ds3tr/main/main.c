/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "main.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#define DPS_TO_RADS (3.14159265f / 180.0f)
#define GRAVITY_CONSTANT 9.80665f
#ifdef CONFIG_BSP_LSM6DS3_SENSOR_ENABLED
TaskHandle_t read_lsm6ds3;
#ifdef CONFIG_BSP_DISPLAY_ENABLED
static lv_obj_t *lsm6ds3_acc_data = NULL;
static lv_obj_t *lsm6ds3_gry_data = NULL;
#endif
#endif
static esp_ldo_channel_handle_t ldo4 = NULL;
static esp_ldo_channel_handle_t ldo3 = NULL;
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_LSM6DS3_SENSOR_ENABLED
#ifdef CONFIG_BSP_DISPLAY_ENABLED
void lsm6ds3_display()
{
    if (lvgl_port_lock(0))
    {
        lsm6ds3_acc_data = lv_label_create(lv_scr_act()); /*Create a label object*/
        static lv_style_t acc_label_style;
        lv_style_init(&acc_label_style);                                                             /*Initialize a style*/
        lv_style_set_bg_opa(&acc_label_style, LV_OPA_TRANSP);                                        /*Set the style LVGL background color*/
        lv_obj_add_style(lsm6ds3_acc_data, &acc_label_style, LV_PART_MAIN);                          /*Add a style to an object*/
        lv_obj_set_style_text_color(lsm6ds3_acc_data, LV_COLOR_WHITE, LV_PART_MAIN);                 /*Set the style LVGL text color*/
        lv_obj_set_style_text_font(lsm6ds3_acc_data, &lv_font_montserrat_30, LV_PART_MAIN);          /*Set the style LVGL text font*/
        lv_obj_align(lsm6ds3_acc_data, LV_ALIGN_CENTER, 0, -30);                                     /*Change the alignment of an object*/
        lv_obj_set_style_bg_color(lv_scr_act(), LV_COLOR_BLACK, LV_PART_MAIN);                       /*Set the screen's LVGL background color*/
        lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, LV_PART_MAIN);                           /*Set the screen's LVGL background transparency*/
        lv_label_set_text(lsm6ds3_acc_data, "acc_x: 0.00 m/s2 acc_y = 0.00 m/s2 acc_z = 0.00 m/s2"); /*Set a new text for a label*/

        lsm6ds3_gry_data = lv_label_create(lv_scr_act()); /*Create a label object*/
        static lv_style_t gry_label_style;
        lv_style_init(&gry_label_style);                                                                /*Initialize a style*/
        lv_style_set_bg_opa(&gry_label_style, LV_OPA_TRANSP);                                           /*Set the style LVGL background color*/
        lv_obj_add_style(lsm6ds3_gry_data, &gry_label_style, LV_PART_MAIN);                             /*Add a style to an object*/
        lv_obj_set_style_text_color(lsm6ds3_gry_data, LV_COLOR_WHITE, LV_PART_MAIN);                    /*Set the style LVGL text color*/
        lv_obj_set_style_text_font(lsm6ds3_gry_data, &lv_font_montserrat_30, LV_PART_MAIN);             /*Set the style LVGL text font*/
        lv_obj_align(lsm6ds3_gry_data, LV_ALIGN_CENTER, 0, 30);                                         /*Change the alignment of an object*/
        lv_obj_set_style_bg_color(lv_scr_act(), LV_COLOR_BLACK, LV_PART_MAIN);                          /*Set the screen's LVGL background color*/
        lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, LV_PART_MAIN);                              /*Set the screen's LVGL background transparency*/
        lv_label_set_text(lsm6ds3_gry_data, "gry_x: 0.00 rad/s gry_y = 0.00 rad/s gry_z = 0.00 rad/s"); /*Set a new text for a label*/

        lvgl_port_unlock();
    }
}

void update_lsm6ds3_value(lsm6ds3tr data)
{
    if ((lsm6ds3_acc_data) && (lsm6ds3_gry_data))
    {
        char buffer_acc[80];
        char buffer_gry[80];
        snprintf(buffer_acc, sizeof(buffer_acc), "acc_x: %.2f m/s2 acc_y = %.2f m/s2 acc_z = %.2f m/s2", data.acc_x * GRAVITY_CONSTANT, data.acc_y * GRAVITY_CONSTANT, data.acc_z * GRAVITY_CONSTANT); /*Format the data into a string*/
        snprintf(buffer_gry, sizeof(buffer_gry), "gry_x: %.2f rad/s gry_y = %.2f rad/s gry_z = %.2f rad/s", data.gry_x * DPS_TO_RADS, data.gry_y * DPS_TO_RADS, data.gry_z * DPS_TO_RADS);             /*Format the data into a string*/
        lv_label_set_text(lsm6ds3_acc_data, buffer_acc);                                                                                                                                               /*Set a new text for a label*/
        lv_label_set_text(lsm6ds3_gry_data, buffer_gry);                                                                                                                                               /*Set a new text for a label*/
    }
}

void lsm6ds3_read_task(void *param)
{
    while (1)
    {
        if (lsm6ds3_scan() != ESP_OK) /*The lsm6ds3 scans to determine whether the currently read data is from the gyroscope or the accelerometer*/
        {
            if (lvgl_port_lock(0))
            {
                lv_label_set_text(lsm6ds3_acc_data, "lsm6ds3 read acc data error"); /*Read failure message displayed*/
                lv_label_set_text(lsm6ds3_gry_data, "lsm6ds3 read gry data error"); /*Read failure message displayed*/
                lvgl_port_unlock();
            }
            MAIN_ERROR("lsm6ds3 scan false");
        }
        else
        {
            if (lvgl_port_lock(0))
            {
                update_lsm6ds3_value(my_lsm6ds3); /*Update the lsm6ds3 data displayed on the screen*/
                lvgl_port_unlock();
            }
        }
        MAIN_INFO("acc_x: %.2f m/s2", my_lsm6ds3.acc_x);
        MAIN_INFO("acc_y: %.2f m/s2", my_lsm6ds3.acc_y);
        MAIN_INFO("acc_z: %.2f m/s2", my_lsm6ds3.acc_z);
        MAIN_INFO("gry_x: %.2f rad/s", my_lsm6ds3.gry_x);
        MAIN_INFO("gry_y: %.2f rad/s", my_lsm6ds3.gry_y);
        MAIN_INFO("gry_z: %.2f rad/s", my_lsm6ds3.gry_z);
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
#ifdef CONFIG_BSP_LSM6DS3_SENSOR_ENABLED
    err = lsm6ds3_begin(); /*Lsm6ds3tr Initialization*/
    if (err != ESP_OK)
        init_fail("lsm6ds3tr", err);
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
#ifdef CONFIG_BSP_LSM6DS3_SENSOR_ENABLED
#ifdef CONFIG_BSP_DISPLAY_ENABLED
    set_lcd_blight(100);                                                                                 /*Set the screen backlight to maximum brightness*/
    lsm6ds3_display();                                                                                   /*Set the screen's LVGL default display page*/
    xTaskCreate(lsm6ds3_read_task, "read_lsm6ds3", 4096, NULL, configMAX_PRIORITIES - 5, &read_lsm6ds3); /*Create a lsm6ds3tr data display refresh thread*/
#endif
#endif
}
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/