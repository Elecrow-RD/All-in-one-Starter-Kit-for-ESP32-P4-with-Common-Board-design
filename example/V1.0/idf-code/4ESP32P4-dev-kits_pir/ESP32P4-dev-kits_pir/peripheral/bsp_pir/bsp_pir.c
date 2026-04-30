/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "bsp_pir.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#ifdef CONFIG_BSP_PIR_ENABLED
static bool PIR_flag = false; /*Initialize the status flag*/
#endif
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_PIR_ENABLED

static void IRAM_ATTR PIR_ISR(void *arg) /*Rising Edge Interrupt Handler for the PIR Pin*/
{
    if ((arg == (void *)PIR_GPIO) && (!PIR_flag))
        PIR_flag = true;
}

esp_err_t pir_init()
{
    esp_err_t err = ESP_OK;
    const gpio_config_t gpio_cofig = {
        .pin_bit_mask = 1ULL << PIR_GPIO, /* GPIO pin: set with bit mask, each bit maps to a GPIO */
        .mode = GPIO_MODE_INPUT,          /* GPIO mode: set input/output mode                     */
        .pull_up_en = false,              /* GPIO pull-up                                         */
        .pull_down_en = false,            /* GPIO pull-down                                       */
        .intr_type = GPIO_INTR_POSEDGE,   /* GPIO interrupt type                                  */
    };
    err = gpio_config(&gpio_cofig); /*Configure GPIO*/
    if (err != ESP_OK)
        return err;
    err = gpio_isr_handler_add(PIR_GPIO, PIR_ISR, (void *)PIR_GPIO); /*Add ISR handler for the corresponding GPIO pin*/
    if (err != ESP_OK)
        return err;
    return err;
}

esp_err_t get_pir_state()
{
    if (PIR_flag)
    {
        if (gpio_get_level(PIR_GPIO)) /*Read the current corresponding level of GPIO*/
            return ESP_OK;
        else
        {
            PIR_flag = false;
            return ESP_FAIL;
        }
    }
    else
        return ESP_FAIL;
    return ESP_FAIL;
}

#endif
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/