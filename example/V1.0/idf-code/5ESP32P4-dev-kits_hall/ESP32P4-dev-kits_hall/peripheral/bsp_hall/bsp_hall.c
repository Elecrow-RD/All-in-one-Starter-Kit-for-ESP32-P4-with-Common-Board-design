/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "bsp_hall.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#ifdef CONFIG_BSP_HALL_ENABLED
static bool hall_state = false; /*Initialize the status flag*/
#endif
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_HALL_ENABLED

static void IRAM_ATTR HALL_ISR(void *arg) /*Hall Pin's Full-Edge Interrupt Handler*/
{
    if ((arg == (void *)HALL_GPIO) && (!gpio_get_level(HALL_GPIO)))
        hall_state = true;
    else if ((arg == (void *)HALL_GPIO) && (gpio_get_level(HALL_GPIO)))
        hall_state = false;
}

esp_err_t hall_init()
{
    esp_err_t err = ESP_OK;
    const gpio_config_t gpio_cofig = {
        .pin_bit_mask = 1ULL << HALL_GPIO, /* GPIO pin: set with bit mask, each bit maps to a GPIO */
        .mode = GPIO_MODE_INPUT,           /* GPIO mode: set input/output mode                     */
        .pull_up_en = false,               /* GPIO pull-up                                         */
        .pull_down_en = false,             /* GPIO pull-down                                       */
        .intr_type = GPIO_INTR_ANYEDGE,    /* GPIO interrupt type                                  */
    };
    err = gpio_config(&gpio_cofig); /*Configure GPIO*/
    if (err != ESP_OK)
        return err;
    err = gpio_isr_handler_add(HALL_GPIO, HALL_ISR, (void *)HALL_GPIO); /*Add ISR handler for the corresponding GPIO pin*/
    if (err != ESP_OK)
        return err;
    return err;
}

bool get_hall_status() /*Get the state of the Hall sensor*/
{
    return hall_state;
}
#endif
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/