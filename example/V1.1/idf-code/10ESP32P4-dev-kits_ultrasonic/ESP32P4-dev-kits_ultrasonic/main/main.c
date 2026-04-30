/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "main.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#ifdef CONFIG_BSP_ULTRASONIC_ENABLED
TaskHandle_t ultrasonic;
#ifdef CONFIG_BSP_DISPLAY_ENABLED
static lv_obj_t *distance_label = NULL;
#endif
#endif
static esp_ldo_channel_handle_t ldo4 = NULL;
static esp_ldo_channel_handle_t ldo3 = NULL;
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_ULTRASONIC_ENABLED
#ifdef CONFIG_BSP_DISPLAY_ENABLED
void ultrasonic_display()
{
    if (lvgl_port_lock(0))
    {
        distance_label = lv_label_create(lv_scr_act()); /*Create a label object*/
        static lv_style_t label_style;
        lv_style_init(&label_style);                                                      /*Initialize a style*/
        lv_style_set_bg_opa(&label_style, LV_OPA_TRANSP);                                 /*Set the style LVGL background color*/
        lv_obj_add_style(distance_label, &label_style, LV_PART_MAIN);                     /*Add a style to an object*/
        lv_obj_set_style_text_color(distance_label, LV_COLOR_WHITE, LV_PART_MAIN);        /*Set the style LVGL text color*/
        lv_obj_set_style_text_font(distance_label, &lv_font_montserrat_30, LV_PART_MAIN); /*Set the style LVGL text font*/
        lv_obj_center(distance_label);                                                    /*Align an object to the center on its parent*/
        lv_obj_set_style_bg_color(lv_scr_act(), LV_COLOR_BLACK, LV_PART_MAIN);            /*Set the screen's LVGL background color*/
        lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, LV_PART_MAIN);                /*Set the screen's LVGL background transparency*/
        lv_label_set_text(distance_label, "distance = 0.0 cm");                           /*Set a new text for a label*/
        lvgl_port_unlock();
    }
}

void update_distance_value(float new_distance_cm)
{
    if (distance_label)
    {
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "distance = %.1f cm", new_distance_cm); /*Format the data into a string*/
        lv_label_set_text(distance_label, buffer);                               /*Set a new text for a label*/
    }
}

void ultrasonic_task(void *param)
{
    static esp_err_t err = ESP_OK;
    uint32_t time = 0;
    float distance = 0;
    err = ultrasonic_callback_register(ultrasonic); /*Register the ultrasonic timing capture callback function (Parameter: Task Handle)*/
    if (err != ESP_OK)
    {
        MAIN_INFO("ultrasonic callback register fail");
    }
    while (1)
    {
        err = send_ultrasonic_start(); /*Send the ultrasonic start signal*/
        if (err != ESP_OK)
        {
            continue;
            MAIN_INFO("send ultrasonic start fail");
        }
        if (xTaskNotifyWait(0x00, ULONG_MAX, &time, pdMS_TO_TICKS(1000)) == pdTRUE) /*Wait for the task notification and obtain the transmitted data value*/
        {
            float pulse_width_us = time * (1000000.0 / esp_clk_apb_freq()); /*1s = 1000000us*/
            if (pulse_width_us > 35000)                                     /*If the measured time data exceeds 35 seconds, it is out-of-range data*/
            {
                MAIN_INFO("the distance exceeds the limit");
                MAIN_INFO("the pulse_width_us =%f", pulse_width_us);
                if (lvgl_port_lock(0))
                {
                    lv_label_set_text(distance_label, "the distance exceeds the limit"); /*Set a new text for a label*/
                    lvgl_port_unlock();
                }
            }
            else
            {
                distance = get_ultrasonic_distance(pulse_width_us); /*Convert the collected time data into distance*/
                if (distance == -1)                                 /*If the value is -1, the distance data is consistent with the previous one*/
                {
                    MAIN_INFO("not new distance");
                }
                else
                {
                    MAIN_INFO("distance=%.2f cm", distance);
                    if (lvgl_port_lock(0))
                    {
                        update_distance_value(distance); /*Update the distance data displayed on the screen*/
                        lvgl_port_unlock();
                    }
                }
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
#ifdef CONFIG_BSP_DISPLAY_ENABLED
    err = display_init(); /*Display Initialization*/
    if (err != ESP_OK)
        init_fail("display", err);
#endif
#ifdef CONFIG_BSP_ULTRASONIC_ENABLED
    err = ultrasonic_init(); /*ultrasoni Initialization*/
    if (err != ESP_OK)
        init_fail("ultrasonic", err);
#endif
}

void app_main(void)
{
    MAIN_INFO("----------Demo version----------");
    MAIN_INFO("----------Start the test----------");
    Init();
#ifdef CONFIG_BSP_ULTRASONIC_ENABLED
#ifdef CONFIG_BSP_DISPLAY_ENABLED
    set_lcd_blight(100);                                                                           /*Set the screen backlight to maximum brightness*/
    ultrasonic_display();                                                                          /*Set the screen's LVGL default display page*/
    xTaskCreate(ultrasonic_task, "ultrasonic", 4096, NULL, configMAX_PRIORITIES - 5, &ultrasonic); /*Create an ultrasonic data display refresh thread*/
#endif
#endif
}
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/