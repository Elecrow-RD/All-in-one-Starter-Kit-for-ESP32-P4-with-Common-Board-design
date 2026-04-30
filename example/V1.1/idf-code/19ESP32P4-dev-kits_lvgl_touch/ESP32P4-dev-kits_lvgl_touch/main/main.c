/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "main.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
static esp_ldo_channel_handle_t ldo4 = NULL;
static esp_ldo_channel_handle_t ldo3 = NULL;
static lv_obj_t *test_screen;
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_DISPLAY_ENABLED
static void button_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);              /*Get the event code of an event*/
    lv_obj_t *btn = lv_event_get_target(e);                   /*Get the object originally targeted by the event. It's the same even if the event is bubbled*/
    int led_index = (int)(intptr_t)lv_event_get_user_data(e); /*Get the user_data passed when the event was registered on the object*/

    if (code == LV_EVENT_PRESSED)
    {
        lv_obj_add_state(btn, LV_STATE_PRESSED); /*Add one or more states to the object*/
        set_single_led_status(led_index, 0xFF);  /*Light up according to the color corresponding to the key press*/
        MAIN_INFO("BTN[%d] pressed", led_index);
    }
    else if (code == LV_EVENT_RELEASED)
    {
        lv_obj_clear_state(btn, LV_STATE_PRESSED); /*Add one or more states to the object*/
        set_single_led_status(led_index, 0x00);
        MAIN_INFO("BTN[%d] released", led_index);
    }
}

void display_test()
{
    if (lvgl_port_lock(0))
    {
        test_screen = lv_obj_create(NULL);
        lv_obj_set_style_bg_color(test_screen, LV_COLOR_BLACK, LV_PART_MAIN); /*Set the screen's LVGL background color*/
        lv_obj_set_style_bg_opa(test_screen, LV_OPA_COVER, LV_PART_MAIN);     /*Set the screen's LVGL background transparency*/
        lv_obj_t *label = lv_label_create(test_screen);                       /*Create a label object*/
        lv_label_set_text(label, "Hello P4!");                                /*Set a new text for a label. Memory will be allocated to store the text by the label.*/
        static lv_style_t label_style;
        lv_style_init(&label_style);                                             /*Initialize a style*/
        lv_style_set_bg_opa(&label_style, LV_OPA_TRANSP);                        /*Set the style LVGL background color*/
        lv_obj_add_style(label, &label_style, LV_PART_MAIN);                     /*Add a style to an object*/
        lv_obj_align(label, LV_ALIGN_CENTER, 0, -100);                           /*Change the alignment of an object*/
        lv_obj_set_style_text_color(label, LV_COLOR_WHITE, LV_PART_MAIN);        /*Set the style LVGL text color*/
        lv_obj_set_style_text_font(label, &lv_font_montserrat_42, LV_PART_MAIN); /*Set the style LVGL text font*/
        lv_obj_t *buttons[4];
        lv_color_t btn_color[4] = {LV_COLOR_RED, LV_COLOR_YELLOW, LV_COLOR_GREEN, LV_COLOR_BLUE};
        const char *button_texts[] = {"RED", "YELLOW", "GREEN", "BLUE"};
        for (int i = 0; i < 4; i++)
        {
            buttons[i] = lv_btn_create(test_screen);                                     /*Create button object*/
            lv_obj_set_size(buttons[i], 100, 100);                                       /*Set the size of an object*/
            lv_obj_set_pos(buttons[i], (162 + (i * 200)), 300);                          /*Set the position of an object relative to the set alignment*/
            lv_obj_set_style_radius(buttons[i], 15, LV_STATE_DEFAULT);                   /*Set rounded corners for display*/
            lv_obj_t *label = lv_label_create(buttons[i]);                               /*Create a label object*/
            lv_label_set_text(label, button_texts[i]);                                   /*Set a new text for a label. Memory will be allocated to store the text by the label.*/
            lv_obj_set_style_text_font(label, &lv_font_montserrat_20, LV_PART_MAIN);     /*Set the style LVGL text font*/
            lv_obj_center(label);                                                        /*Align an object to the center on its parent*/
            lv_obj_set_style_bg_color(buttons[i], btn_color[i], LV_STATE_DEFAULT);       /*Set the style LVGL background color*/
            lv_obj_set_style_text_color(buttons[i], LV_COLOR_BLACK, LV_STATE_DEFAULT);   /*Set the style LVGL text color*/
            lv_obj_set_style_border_width(buttons[i], 2, LV_STATE_DEFAULT);              /*Set the border width of the object*/
            lv_obj_set_style_border_color(buttons[i], LV_COLOR_BLACK, LV_STATE_DEFAULT); /*Set the border color of the object*/
            lv_obj_set_style_bg_color(buttons[i], btn_color[i], LV_STATE_PRESSED);       /*Set the style LVGL background color - Pressed State*/
            lv_obj_set_style_text_color(buttons[i], LV_COLOR_BLACK, LV_STATE_PRESSED);   /*Set the style LVGL text color - Pressed State*/
            lv_obj_set_style_border_color(buttons[i], LV_COLOR_GRAY, LV_STATE_PRESSED);  /*Set the border color of the object - Pressed State*/

            lv_obj_add_event_cb(buttons[i], button_event_handler, LV_EVENT_ALL, (void *)(intptr_t)i); /*Add event handling and pass the LED index as user data*/
        }
        lv_scr_load(test_screen); /*Set the current active screen*/
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
#ifdef CONFIG_BSP_I2C_ENABLED
    err = i2c_init(); /*I2C Initialization*/
    if (err != ESP_OK)
        init_fail("i2c", err);
    vTaskDelay(200 / portTICK_PERIOD_MS);
#endif
#ifdef CONFIG_BSP_LED_ENABLED
    err = led_init(); /*RMT LED Initialization*/
    if (err != ESP_OK)
        init_fail("led", err);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    set_led_status(0x00000000); /*All the LEDs are off*/
#endif
#ifdef CONFIG_BSP_DISPLAY_ENABLED
#if defined(CONFIG_BSP_TOUCH_ENABLED) && defined(CONFIG_BSP_I2C_ENABLED)
    err = touch_init(); /*touch Initialization*/
    if (err != ESP_OK)
        init_fail("display touch", err);
#endif
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
#ifdef CONFIG_BSP_DISPLAY_ENABLED
    set_lcd_blight(100);   /*Set the screen backlight to maximum brightness*/
    if (lvgl_port_lock(0)) /*Take LVGL mutex*/
    {
        display_test();     /*Set the screen's LVGL default display page*/
        lvgl_port_unlock(); /*Give LVGL mutex*/
    }
#endif
}
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/