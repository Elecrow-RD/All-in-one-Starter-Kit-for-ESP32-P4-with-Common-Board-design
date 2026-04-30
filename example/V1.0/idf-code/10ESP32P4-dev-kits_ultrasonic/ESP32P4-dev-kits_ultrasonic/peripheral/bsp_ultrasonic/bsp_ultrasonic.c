/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "bsp_ultrasonic.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#ifdef CONFIG_BSP_ULTRASONIC_ENABLED
static ultrasonic_struct ultrasonic_data;   /*Ultrasonic Data Structure*/
mcpwm_cap_channel_handle_t cap_chan = NULL; /*Type of MCPWM capture channel handle*/
#endif
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_ULTRASONIC_ENABLED

static bool hc_sr04_echo_callback(mcpwm_cap_channel_handle_t cap_chan, const mcpwm_capture_event_data_t *edata, void *user_data)
{
    TaskHandle_t task_to_notify = (TaskHandle_t)user_data;
    BaseType_t high_task_wakeup = pdFALSE;
    if (edata->cap_edge == MCPWM_CAP_EDGE_POS)
    {
        ultrasonic_data.ultrasonic_start_time = edata->cap_value;
        ultrasonic_data.ultrasonic_stop_time = ultrasonic_data.ultrasonic_start_time;
    }
    else
    {
        ultrasonic_data.ultrasonic_stop_time = edata->cap_value;
        ultrasonic_data.ultrasonic_time = ultrasonic_data.ultrasonic_stop_time - ultrasonic_data.ultrasonic_start_time;
        xTaskNotifyFromISR(task_to_notify, ultrasonic_data.ultrasonic_time, eSetValueWithOverwrite, &high_task_wakeup);
    }
    return high_task_wakeup == pdTRUE;
}

esp_err_t ultrasonic_init()
{
    esp_err_t err = ESP_OK;
    mcpwm_cap_timer_handle_t cap_timer = NULL; /*Type of MCPWM capture timer handle*/
    mcpwm_capture_timer_config_t cap_conf = {
        .clk_src = MCPWM_CAPTURE_CLK_SRC_DEFAULT, /*MCPWM capture timer clock source*/
        .group_id = 0,                            /*Specify from which group to allocate the capture timer*/
    }; /*MCPWM capture timer configuration structure*/
    err = mcpwm_new_capture_timer(&cap_conf, &cap_timer); /*Create MCPWM capture timer*/
    if (err != ESP_OK)
        return err;
    mcpwm_capture_channel_config_t cap_ch_conf = {
        .gpio_num = ULTRASONIC_GPIO_ECHO, /*GPIO used capturing input signal*/
        .prescale = 1,                    /*Prescale of input signal, effective frequency = cap_input_clk/prescale*/
        .flags.neg_edge = true,           /*Whether to capture on negative edge*/
        .flags.pos_edge = true,           /*Whether to capture on positive edge*/
        .flags.pull_up = true,            /*Whether to pull up internally*/
    }; /*MCPWM capture channel configuration structure*/
    err = mcpwm_new_capture_channel(cap_timer, &cap_ch_conf, &cap_chan); /*Create MCPWM capture channel*/
    if (err != ESP_OK)
        return err;
    const gpio_config_t gpio_cofig = {
        .pin_bit_mask = 1ULL << ULTRASONIC_GPIO_TRIG, /* GPIO pin: set with bit mask, each bit maps to a GPIO */
        .mode = GPIO_MODE_OUTPUT,                     /* GPIO mode: set input/output mode                     */
        .pull_up_en = false,                          /* GPIO pull-up                                         */
        .pull_down_en = false,                        /* GPIO pull-down                                       */
        .intr_type = GPIO_INTR_DISABLE,               /* GPIO interrupt type                                  */
    };
    err = gpio_config(&gpio_cofig); /*Configure GPIO*/
    if (err != ESP_OK)
        return err;
    err = mcpwm_capture_timer_enable(cap_timer); /*Enable MCPWM capture timer*/
    if (err != ESP_OK)
        return err;
    err = mcpwm_capture_timer_start(cap_timer); /*Start MCPWM capture timer*/
    if (err != ESP_OK)
        return err;
    ultrasonic_data.ultrasonic_start_time = 0;
    ultrasonic_data.ultrasonic_stop_time = 0;
    ultrasonic_data.ultrasonic_time = 0;
    return err;
}

esp_err_t ultrasonic_callback_register(TaskHandle_t handle)
{
    esp_err_t err = ESP_OK;
    /*Group of supported MCPWM capture event callbacks*/
    mcpwm_capture_event_callbacks_t cbs = {
        .on_cap = hc_sr04_echo_callback, /*Callback function that would be invoked when capture event occurred*/
    };
    err = mcpwm_capture_channel_register_event_callbacks(cap_chan, &cbs, handle); /*Set event callbacks for MCPWM capture channel*/
    if (err != ESP_OK)
        return err;
    err = mcpwm_capture_channel_enable(cap_chan); /*Enable MCPWM capture channel*/
    if (err != ESP_OK)
        return err;
    return err;
}

esp_err_t ultrasonic_channel_disable()
{
    esp_err_t err = ESP_OK;
    err = mcpwm_capture_channel_disable(cap_chan); /*Disable MCPWM capture channel*/
    if (err != ESP_OK)
        return err;
    return err;
}

esp_err_t send_ultrasonic_start()
{
    esp_err_t err = ESP_OK;
    err = gpio_set_level(ULTRASONIC_GPIO_TRIG, 0); /*Set the Corresponding Output Level of GPIO*/
    if (err != ESP_OK)
        return err;
    ets_delay_us(20);
    err = gpio_set_level(ULTRASONIC_GPIO_TRIG, 1);
    if (err != ESP_OK)
        return err;
    ets_delay_us(10);
    err = gpio_set_level(ULTRASONIC_GPIO_TRIG, 0);
    if (err != ESP_OK)
        return err;
    return err;
}

float get_ultrasonic_distance(uint32_t time)
{
    static float last_ultrasonic_distance = 0;
    ultrasonic_data.ultrasonic_distance = time * 0.01715;                /*Ultrasonic Conversion Formula*/
    if (ultrasonic_data.ultrasonic_distance != last_ultrasonic_distance) /*If the currently converted data is inconsistent with the previous data*/
    {
        last_ultrasonic_distance = ultrasonic_data.ultrasonic_distance;
        return ultrasonic_data.ultrasonic_distance;
    }
    last_ultrasonic_distance = ultrasonic_data.ultrasonic_distance;
    return -1; /*If the currently converted data is consistent with the previous data*/
}
#endif
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/