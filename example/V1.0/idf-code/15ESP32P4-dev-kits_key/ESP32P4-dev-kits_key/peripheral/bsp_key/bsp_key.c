/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "bsp_key.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#ifdef CONFIG_BSP_KEY_ENABLED
button_handle_t btns[4] = {NULL};
key_state key_status = Key_idle;
#endif
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_KEY_ENABLED

static void button_event_cb(void *arg, void *data)
{
    button_event_t event = iot_button_get_event(arg); /*Get button event*/
    switch (event)
    {
    case BUTTON_PRESS_DOWN: /*Press the button*/
        switch ((int)data)  /*Determine the key sequence identifier*/
        {
        case 0:
            key_status = Key_left;
            KEY_INFO("left btn : BUTTON_PRESS_DOWN");
            break;
        case 1:
            key_status = Key_right;
            KEY_INFO("right btn : BUTTON_PRESS_DOWN");
            break;
        case 2:
            key_status = Key_middle;
            KEY_INFO("middle btn : BUTTON_PRESS_DOWN");
            break;
        case 3:
            key_status = Key_up;
            KEY_INFO("up btn : BUTTON_PRESS_DOWN");
            break;
        }
        break;
    case BUTTON_PRESS_UP: /*Release the button*/
        switch ((int)data)
        {
        case 0:
            KEY_INFO("left btn : BUTTON_PRESS_UP");
            break;
        case 1:
            KEY_INFO("right btn : BUTTON_PRESS_UP");
            break;
        case 2:
            KEY_INFO("middle btn : BUTTON_PRESS_UP");
            break;
        case 3:
            KEY_INFO("up btn : BUTTON_PRESS_UP");
            break;
        }
        key_status = Key_release;
        break;
    default:
        break;
    }
}

esp_err_t key_init(adc_oneshot_unit_handle_t handle)
{
    esp_err_t err = ESP_OK;
    /*Configure button timing parameters*/
    button_config_t btn_cfg = {
        .long_press_time = 3000, /*Trigger time(ms) for long press*/
        .short_press_time = 200, /*Trigger time(ms) for short press*/
    };
    /*Configure ADC parameters for 4 buttons on the same ADC channel*/
    button_adc_config_t btn_adc_cfg[4] = {
        /*Button 0: Voltage range 1500-1900*/
        {
            .adc_handle = &handle,        /*Pointer to ADC unit handle*/
            .unit_id = ADC_UNIT_1,        /*Use ADC unit 1*/
            .adc_channel = ADC_CHANNEL_0, /*Use ADC channel 0*/
            .button_index = 0,            /*Identifier for button 0*/
            .min = 1500,                  /*Minimum ADC value for button 0 detection*/
            .max = 1900,                  /*Maximum ADC value for button 0 detection*/
        },
        /*Button 1: Voltage range 2000-2320*/
        {
            .adc_handle = &handle,
            .unit_id = ADC_UNIT_1,
            .adc_channel = ADC_CHANNEL_0,
            .button_index = 1,
            .min = 2000, /*Minimum ADC value for button 1 detection*/
            .max = 2320, /*Maximum ADC value for button 1 detection*/
        },
        /*Button 2: Voltage range 2400-2650*/
        {
            .adc_handle = &handle,
            .unit_id = ADC_UNIT_1,
            .adc_channel = ADC_CHANNEL_0,
            .button_index = 2,
            .min = 2400, /*Minimum ADC value for button 2 detection*/
            .max = 2650, /*Maximum ADC value for button 2 detection*/
        },
        /*Button 3: Voltage range 2700-3000*/
        {
            .adc_handle = &handle,
            .unit_id = ADC_UNIT_1,
            .adc_channel = ADC_CHANNEL_0,
            .button_index = 3,
            .min = 2700, /*Minimum ADC value for button 3 detection*/
            .max = 3000, /*Maximum ADC value for button 3 detection*/
        },
    };
    for (size_t i = 0; i < 4; i++)
    {
        err = iot_button_new_adc_device(&btn_cfg, &btn_adc_cfg[i], &btns[i]); /*Create a new ADC button device*/
        if (err != ESP_OK)
            return err;
        if (btns[i] == NULL) /*Verify that the button handle was created successfully*/
            return err;
    }
    return err;
}

esp_err_t key_register_cb()
{
    esp_err_t err = ESP_OK;
    for (size_t i = 0; i < 4; i++)
    {
        err = iot_button_register_cb(btns[i], BUTTON_PRESS_DOWN, NULL, button_event_cb, (void *)i); /*Register the button event callback function*/
        if (err != ESP_OK)
            return err;
        err = iot_button_register_cb(btns[i], BUTTON_PRESS_UP, NULL, button_event_cb, (void *)i);
        if (err != ESP_OK)
            return err;
    }
    return err;
}
#endif
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/