/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "main.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#ifdef CONFIG_BSP_KEY_ENABLED
TaskHandle_t key;
#endif

static esp_ldo_channel_handle_t ldo4 = NULL;
static esp_ldo_channel_handle_t ldo3 = NULL;
adc_oneshot_unit_handle_t adc_handle = NULL;
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_KEY_ENABLED
#ifdef CONFIG_BSP_LED_ENABLED
void key_task(void *param)
{
    while (1)
    {
        /*Process key events based on the current key_status*/
        switch (key_status)
        {
        case Key_idle:
            /*No key pressed*/
            vTaskDelay(10 / portTICK_PERIOD_MS);
            break;
        case Key_left:
            if (set_single_led_status(LED_BYTE_RED, 0xFF) != ESP_OK) /*Left button pressed - set LED to red*/
            {
                MAIN_ERROR("Failed to set LED status");
            }
            key_status = Key_idle;
            break;
        case Key_right:
            if (set_single_led_status(LED_BYTE_BLUE, 0xFF) != ESP_OK) /*Right button pressed - set LED to blue*/
            {
                MAIN_ERROR("Failed to set LED status");
            }
            key_status = Key_idle;
            break;
        case Key_middle:
            if (set_single_led_status(LED_BYTE_YELLOW, 0xFF) != ESP_OK) /*middle button pressed - set LED to yellow*/
            {
                MAIN_ERROR("Failed to set LED status");
            }
            key_status = Key_idle;
            break;
        case Key_up:
            if (set_single_led_status(LED_BYTE_GREEN, 0xFF) != ESP_OK) /*up button pressed - set LED to green*/
            {
                MAIN_ERROR("Failed to set LED status");
            }
            key_status = Key_idle;
            break;
        case Key_release:                             /*Key released - turn off all LEDs*/
            if (set_led_status(0x00000000) != ESP_OK) /*All the LEDs are off*/
            {
                MAIN_ERROR("Failed to set LED status");
            }
            key_status = Key_idle;
            break;
        default:
            MAIN_ERROR("Unknown key status: %d", key_status);
            key_status = Key_idle;
            break;
        }
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
    adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id = ADC_UNIT_1,
    };
    err = adc_oneshot_new_unit(&init_cfg, &adc_handle); /*Create a handle to a specific ADC unit*/
    if (err != ESP_OK)
        init_fail("new adc oneshot", err);
#ifdef CONFIG_BSP_LED_ENABLED
    err = led_init(); /*RMT LED Initialization*/
    if (err != ESP_OK)
        init_fail("led", err);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    set_led_status(0x00000000); /*All the LEDs are off*/
#endif
#ifdef CONFIG_BSP_KEY_ENABLED
    err = key_init(adc_handle); /*ADC Key Initialization*/
    if (err != ESP_OK)
        init_fail("key", err);
    err = key_register_cb(); /*Key press status registration callback*/
    if (err != ESP_OK)
        init_fail("key register cb", err);
#endif
}

void app_main(void)
{
    MAIN_INFO("----------Demo version----------");
    MAIN_INFO("----------Start the test----------");
    Init();
#ifdef CONFIG_BSP_KEY_ENABLED
#ifdef CONFIG_BSP_LED_ENABLED
    xTaskCreate(key_task, "key", 4096, NULL, configMAX_PRIORITIES - 5, &key); /*Create a thread for turning on the LED light by pressing a button*/
#endif
#endif
}
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/
