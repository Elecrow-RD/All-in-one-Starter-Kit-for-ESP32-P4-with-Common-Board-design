/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "bsp_led.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_LED_ENABLED
esp_err_t led_init()
{
    esp_err_t err = ESP_OK;
    const gpio_config_t gpio_cofig = {
        .pin_bit_mask = 1ULL << LED_GPIO, /* GPIO pin: set with bit mask, each bit maps to a GPIO */
        .mode = GPIO_MODE_INPUT_OUTPUT,   /* GPIO mode: set input/output mode                     */
        .pull_up_en = false,              /* GPIO pull-up                                         */
        .pull_down_en = false,            /* GPIO pull-down                                       */
        .intr_type = GPIO_INTR_DISABLE,   /* GPIO interrupt type                                  */
    };
    err = gpio_config(&gpio_cofig); /*Configure GPIO*/
    if (err != ESP_OK)
        return err;
    return err;
}

esp_err_t set_led_state(bool state)
{
    if (gpio_set_level(LED_GPIO, state) != ESP_OK)
        return ESP_FAIL;
    return ESP_OK;
}

void led_toggle()
{
    gpio_set_level(LED_GPIO, !(gpio_get_level(LED_GPIO))); /*Set the Corresponding Output Level of GPIO*/
}
#endif
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/