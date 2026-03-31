/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "bsp_servo.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_SERVO_ENABLED

static inline int16_t clamp_i16(int16_t val, int16_t min, int16_t max)
{
    if (val < min)
        return min;
    if (val > max)
        return max;
    return val;
}

static uint32_t calculate_servo_duty(int degree)
{
    degree = clamp_i16(degree, 0, SERVO_MAX_DEGREE);
    uint32_t pulsewidth = SERVO_MIN_PULSEWIDTH_US + (((SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) * degree) / SERVO_MAX_DEGREE);
    uint32_t duty = (pulsewidth * (1 << 14)) / 20000;
    return duty;
}

bool parse_angle_command(const char *str, int *out_angle)
{
    if (str == NULL || strlen(str) == 0)
        return false;

    if (str[0] == 'a')
    {
        *out_angle = atoi(&str[1]);
        return true;
    }
    return false;
}

esp_err_t servo_init()
{
    esp_err_t err = ESP_OK;
    const gpio_config_t gpio_cofig = {
        .pin_bit_mask = 1ULL << SERVO_GPIO,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = false,
        .pull_down_en = false,
        .intr_type = GPIO_INTR_DISABLE,
    };
    err = gpio_config(&gpio_cofig);
    if (err != ESP_OK)
        return err;
    const ledc_timer_config_t servo_timer_config = {
        .clk_cfg = LEDC_USE_PLL_DIV_CLK,
        .duty_resolution = LEDC_TIMER_14_BIT,
        .freq_hz = 50,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_1,
    };
    const ledc_channel_config_t channel_config = {
        .gpio_num = SERVO_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_1,
        .duty = calculate_servo_duty(0),
        .hpoint = 0,
    };
    err = ledc_timer_config(&servo_timer_config);
    if (err != ESP_OK)
        return err;
    err = ledc_channel_config(&channel_config);
    if (err != ESP_OK)
        return err;
    return err;
}

esp_err_t set_servo_angle(int degree)
{
    esp_err_t err = ESP_OK;
    uint32_t duty = calculate_servo_duty(degree);
    err = ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, duty);
    if (err != ESP_OK)
        return err;
    err = ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
    if (err != ESP_OK)
        return err;
    return err;
}

#endif
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/