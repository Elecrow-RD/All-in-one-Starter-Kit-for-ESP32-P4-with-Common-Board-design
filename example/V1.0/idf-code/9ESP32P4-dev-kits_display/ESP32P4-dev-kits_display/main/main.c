/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "main.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
static esp_ldo_channel_handle_t ldo4 = NULL;
static esp_ldo_channel_handle_t ldo3 = NULL;
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_DISPLAY_ENABLED
void display_test()
{
    if (lvgl_port_lock(0))
    {
        lv_obj_t *label = lv_label_create(lv_scr_act()); /*Create a label object*/
        lv_label_set_text(label, "Hello P4!");           /*Set a new text for a label. Memory will be allocated to store the text by the label.*/

        static lv_style_t label_style;
        lv_style_init(&label_style);                                             /*Initialize a style*/
        lv_style_set_bg_opa(&label_style, LV_OPA_TRANSP);                        /*Set the style LVGL background color*/
        lv_obj_add_style(label, &label_style, LV_PART_MAIN);                     /*Add a style to an object*/
        lv_obj_center(label);                                                    /*Align an object to the center on its parent*/
        lv_obj_set_style_text_color(label, LV_COLOR_BLACK, LV_PART_MAIN);        /*Set the style LVGL text color*/
        lv_obj_set_style_text_font(label, &lv_font_montserrat_42, LV_PART_MAIN); /*Set the style LVGL text font*/

        lv_obj_set_style_bg_color(lv_scr_act(), LV_COLOR_BLACK, LV_PART_MAIN); /*Set the screen's LVGL background color*/
        lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, LV_PART_MAIN);     /*Set the screen's LVGL background transparency*/
        lvgl_port_unlock();
    }
}
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
}

void app_main(void)
{
    MAIN_INFO("----------Demo version----------");
    Init();
#ifdef CONFIG_BSP_DISPLAY_ENABLED
    set_lcd_blight(100);   /*Set the screen backlight to maximum brightness*/
    if (lvgl_port_lock(0)) /*Take LVGL mutex*/
    {
        display_test();     /*Set the screen's LVGL default display page*/
        lvgl_port_unlock(); /*Give LVGL mutex*/
    }
    while (1)
    {
        if (lvgl_port_lock(0))
        {
            lv_obj_set_style_bg_color(lv_scr_act(), LV_COLOR_RED, LV_PART_MAIN); /*Set the screen's LVGL background color*/
            lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, LV_PART_MAIN);   /*Set the screen's LVGL background transparency*/
            lvgl_port_unlock();
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
        if (lvgl_port_lock(0))
        {
            lv_obj_set_style_bg_color(lv_scr_act(), LV_COLOR_YELLOW, LV_PART_MAIN);
            lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, LV_PART_MAIN);
            lvgl_port_unlock();
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
        if (lvgl_port_lock(0))
        {
            lv_obj_set_style_bg_color(lv_scr_act(), LV_COLOR_BLUE, LV_PART_MAIN);
            lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, LV_PART_MAIN);
            lvgl_port_unlock();
        }
        vTaskDelay(pdMS_TO_TICKS(2000)); /*Delay 2000ms*/
    }
#endif
    MAIN_INFO("----------Start the test----------");
}
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/