/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "bsp_touch.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_INDEPENDENT_TOUCH_ENABLED
esp_err_t independent_touch_init()
{
    esp_err_t err = ESP_OK;
    const gpio_config_t gpio_cofig = {
        .pin_bit_mask = 1ULL << INDEPENDENT_TOUCH_GPIO, /* GPIO pin: set with bit mask, each bit maps to a GPIO */
        .mode = GPIO_MODE_INPUT,                        /* GPIO mode: set input/output mode                     */
        .pull_up_en = false,                            /* GPIO pull-up                                         */
        .pull_down_en = false,                          /* GPIO pull-down                                       */
        .intr_type = GPIO_INTR_DISABLE,                 /* GPIO interrupt type                                  */
    };
    err = gpio_config(&gpio_cofig); /*Configure GPIO*/
    if (err != ESP_OK)
        return err;
    return err;
}

int get_touch_state()
{
    return gpio_get_level(INDEPENDENT_TOUCH_GPIO); /*Read the current corresponding level of GPIO*/
}
#endif
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/