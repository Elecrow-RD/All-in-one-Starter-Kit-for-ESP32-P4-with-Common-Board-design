/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "bsp_servo.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_SERVO_ENABLED

esp_err_t servo_init()
{
    esp_err_t err = ESP_OK;
    const gpio_config_t gpio_cofig = {
        .pin_bit_mask = 1ULL << SERVO_GPIO, /* GPIO pin: set with bit mask, each bit maps to a GPIO */
        .mode = GPIO_MODE_OUTPUT,           /* GPIO mode: set input/output mode                     */
        .pull_up_en = false,                /* GPIO pull-up                                         */
        .pull_down_en = false,              /* GPIO pull-down                                       */
        .intr_type = GPIO_INTR_DISABLE,     /* GPIO interrupt type                                  */
    };
    err = gpio_config(&gpio_cofig); /*Configure GPIO*/
    if (err != ESP_OK)
        return err;

    const ledc_timer_config_t servo_timer_config = {
        .clk_cfg = LEDC_USE_PLL_DIV_CLK,      /* Configure LEDC source clock from ledc_clk_cfg_t*/
        .duty_resolution = LEDC_TIMER_11_BIT, /*LEDC channel duty resolution*/
        .freq_hz = 50,                        /*LEDC timer frequency (Hz)*/
        .speed_mode = LEDC_LOW_SPEED_MODE,    /*LEDC speed speed_mode, high-speed mode (only exists on esp32) or low-speed mode*/
        .timer_num = LEDC_TIMER_1,            /* The timer source of channel (0 - LEDC_TIMER_MAX-1)*/
    };
    const ledc_channel_config_t channel_config = {
        .gpio_num = SERVO_GPIO,            /*the LEDC output gpio_num*/
        .speed_mode = LEDC_LOW_SPEED_MODE, /*LEDC speed speed_mode, high-speed mode (only exists on esp32) or low-speed mode*/
        .channel = LEDC_CHANNEL_1,         /*LEDC channel (0 - LEDC_CHANNEL_MAX-1)*/
        .intr_type = LEDC_INTR_DISABLE,    /*configure interrupt, Fade interrupt enable  or Fade interrupt disable*/
        .timer_sel = LEDC_TIMER_1,         /*Select the timer source of channel (0 - LEDC_TIMER_MAX-1)*/
        .duty = 154,                       /*EDC channel duty, the range of duty setting is [0, (2**duty_resolution)] */
        .hpoint = 0,                       /*LEDC channel hpoint value, the range is [0, (2**duty_resolution)-1]*/
    };
    err = ledc_timer_config(&servo_timer_config); /*LEDC timer configuration Configure LEDC timer with the given source timer/frequency(Hz)/duty_resolution*/
    if (err != ESP_OK)
        return err;
    err = ledc_channel_config(&channel_config); /*LEDC channel configuration Configure LEDC channel with the given channel/output gpio_num/interrupt/source timer/frequency(Hz)/LEDC dut*/
    if (err != ESP_OK)
        return err;
    return err;
}

/*speed -  (0 - 4(fastest)) */
esp_err_t set_servo_status(servo_dir servo_dir_status, int speed)
{
    esp_err_t err = ESP_OK;
    if ((speed < 0) || (speed > 4)) /*The speed is not within the specified range*/
        return ESP_ERR_INVALID_ARG;
    switch (servo_dir_status) /**/
    {
    case stop:
        err = ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, 154); /*LEDC set duty*/
        if (err != ESP_OK)
            return err;
        err = ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1); /*LEDC update channel parameters*/
        if (err != ESP_OK)
            return err;
        break;
    case forward_dir:
        err = ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, (132 - (speed * 20)));
        if (err != ESP_OK)
            return err;
        err = ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
        if (err != ESP_OK)
            return err;
        break;
    case reverse_dir:
        err = ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, (speed * 20) + 176);
        if (err != ESP_OK)
            return err;
        err = ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
        if (err != ESP_OK)
            return err;
        break;
    }
    return err;
}

#endif
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/