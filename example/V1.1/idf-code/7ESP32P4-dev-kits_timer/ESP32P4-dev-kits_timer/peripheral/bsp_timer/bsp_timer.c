/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "bsp_timer.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#ifdef CONFIG_BSP_TIMER_ENABLED
esp_timer_handle_t periodic_timer;
static bool timer_flag = false; /*Initialize the timer flag*/
#endif
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_TIMER_ENABLED
void periodic_timer_callback(void *arg) /*Timer Interrupt Handler*/
{
    if (timer_flag == false)
        timer_flag = true;
}

esp_err_t timer_init()
{
    esp_err_t err = ESP_OK;

    /*Timer configuration passed to esp_timer_create()*/
    const esp_timer_create_args_t timer_args = {
        .callback = &periodic_timer_callback, /* Callback function to execute when timer expires*/
        .arg = NULL,                          /*Argument to pass to callback*/
        .name = "Mytimer"                     /*Timer name*/
    };

    err = esp_timer_create(&timer_args, &periodic_timer); /*Create an esp_timer instance*/
    if (err != ESP_OK)
        return err;

    return err;
}

esp_err_t start_timer(uint64_t time)
{
    esp_err_t err = ESP_OK;
    err = esp_timer_start_periodic(periodic_timer, time); /*Start a periodic timer(period: us)*/
    if (err != ESP_OK)
    {
        TIMER_ERROR("Start a periodic timer error");
        return err;
    }
    return err;
}

esp_err_t stop_timer(esp_timer_handle_t timer)
{
    esp_err_t err = ESP_OK;
    err = esp_timer_stop(timer); /*Start a periodic timer(period: us)*/
    if (err != ESP_OK)
    {
        TIMER_ERROR("stop a timer error");
        return err;
    }
    return err;
}

bool get_timer_flag() /*Get the timer flag*/
{
    return timer_flag;
}

void reset_timer_flag() /*Reset the timer flag*/
{
    timer_flag = false;
}
#endif
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/