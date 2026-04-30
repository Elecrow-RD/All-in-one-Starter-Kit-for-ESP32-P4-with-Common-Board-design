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
#ifdef CONFIG_BSP_UART_ENABLED
    err = uart_init(); /*Uart Driver Initialization*/
    if (err != ESP_OK)
        init_fail("uart", err);
#endif
}

void app_main(void)
{
    MAIN_INFO("----------Demo version----------");
    MAIN_INFO("----------Start the test--------");
    Init();
#ifdef CONFIG_BSP_UART_ENABLED
#ifdef CONFIG_BSP_LED_ENABLED
    int Rx_Bytes = 0;                     /*Data length read from the serial port buffer*/
    char *data = (char *)malloc(512 + 1); /*Storage space for data read from the serial port buffer*/
    while (1)
    {
        Rx_Bytes = uart_read_bytes(UART_NUM_0, data, 512, pdMS_TO_TICKS(1000)); /*UART read bytes from UART buffer*/
        if (Rx_Bytes > 0)
        {
            data[Rx_Bytes] = 0; /*Append \0 at the end of the string*/
            MAIN_INFO("read data: %s, length: %d", data, Rx_Bytes);
            if (strcmp(data, CMD_LED_ON) == 0) /*Compare whether the string matches the preset command*/
            {
                MAIN_INFO("Received the LED_ON command");
                set_led_state(false); /*Set the Corresponding Output Level of GPIO*/
            }
            else if (strcmp(data, CMD_LED_OFF) == 0) /*Compare whether the string matches the preset command*/
            {
                MAIN_INFO("Received the LED_OFF command");
                set_led_state(true); /*Set the Corresponding Output Level of GPIO*/
            }
            else
            {
                MAIN_ERROR("Unknown Command: %s", data);
            }
        }
    }
#endif
#endif
}
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/