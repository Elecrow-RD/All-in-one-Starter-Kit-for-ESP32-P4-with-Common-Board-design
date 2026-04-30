/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "main.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
static esp_ldo_channel_handle_t ldo4 = NULL;
static esp_ldo_channel_handle_t ldo3 = NULL;
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
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
#ifdef CONFIG_BSP_LED_ENABLED
    err = led_init(); /*LED Driver Initialization*/
    if (err != ESP_OK)
        init_fail("led", err);
#endif
#ifdef CONFIG_BSP_INDEPENDENT_TOUCH_ENABLED
    err = independent_touch_init(); /*Touch Driver Initialization*/
    if (err != ESP_OK)
        init_fail("independent touch", err);
#endif
}

void app_main(void)
{
    MAIN_INFO("----------Demo version----------");
    MAIN_INFO("----------Start the test--------");
    Init();
#ifdef CONFIG_BSP_INDEPENDENT_TOUCH_ENABLED
#ifdef CONFIG_BSP_LED_ENABLED

    bool state = false;      /*Touch button status*/
    bool last_state = false; /*Previous state of the touch button*/
    while (1)
    {
        state = get_touch_state();            /*Get the current touch button status*/
        if ((state) && (state != last_state)) /*If the previous state of the touch button was released, and the current state of the touch button is pressed*/
        {
            set_led_state(true); /*Set the Corresponding Output Level of GPIO*/
        }
        else if ((!state) && (state != last_state)) /*If the previous state of the touch button was pressed, and the current state of the touch button is released*/
        {
            set_led_state(false); /*Set the Corresponding Output Level of GPIO*/
        }
        last_state = state;                  /*Save the previous touch button state*/
        vTaskDelay(10 / portTICK_PERIOD_MS); /*Delay 10ms*/
    }

#endif
#endif
}
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/