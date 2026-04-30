/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "main.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#ifdef CONFIG_BSP_LED_ENABLED
TaskHandle_t led;
#endif

static esp_ldo_channel_handle_t ldo4 = NULL;
static esp_ldo_channel_handle_t ldo3 = NULL;
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_LED_ENABLED
void led_task(void *param)
{
    led_byte_order_t led_byte = LED_BYTE_RED; /*Initialize the default display color*/
    while (1)
    {
        /*Set the lighting of a single lamp*/
        set_single_led_status(led_byte++, 0xFF); /*led_byte++:cycled through all available colors,0xFF = full brightness (255)*/
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        if (led_byte == LED_BYTE_DONE) /*Check if we've cycled through all available colors*/
        {
            led_byte = LED_BYTE_RED; /*Reset to Red color to restart the cycle*/
        }
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
#ifdef CONFIG_BSP_LED_ENABLED
    err = led_init(); /*RMT LED Initialization*/
    if (err != ESP_OK)
        init_fail("led", err);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    set_led_status(0x00000000); /*All the LEDs are off*/
#endif
}

void app_main(void)
{
    MAIN_INFO("----------Demo version----------");
    MAIN_INFO("----------Start the test----------");
    Init();
#ifdef CONFIG_BSP_LED_ENABLED
    xTaskCreate(led_task, "led", 4096, NULL, configMAX_PRIORITIES - 5, &led); /*Create a led display refresh thread*/
#endif
}
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/