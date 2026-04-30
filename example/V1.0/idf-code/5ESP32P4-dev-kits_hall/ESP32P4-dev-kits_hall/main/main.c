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
    err = gpio_install_isr_service(0); /*Install the GPIO driver's ETS_GPIO_INTR_SOURCE ISR handler service, which allows per-pin GPIO interrupt handlers*/
    if (err != ESP_OK)
        init_fail("gpio isr service", err);
#ifdef CONFIG_BSP_LED_ENABLED
    err = led_init(); /*LED Driver Initialization*/
    if (err != ESP_OK)
        init_fail("led", err);
#endif
#ifdef CONFIG_BSP_HALL_ENABLED
    err = hall_init(); /*HALL Driver Initialization*/
    if (err != ESP_OK)
        init_fail("hall", err);
#endif
}

void app_main(void)
{
    MAIN_INFO("----------Demo version----------");
    MAIN_INFO("----------Start the test----------");
    Init();
#ifdef CONFIG_BSP_HALL_ENABLED
#ifdef CONFIG_BSP_LED_ENABLED
    bool state = false;      /*HALL status*/
    bool last_state = false; /*Previous state of the HALL*/
    if (get_hall_status())   /*First judgment of HALL state*/
        set_led_state(false);
    else
        set_led_state(true);
    while (1)
    {
        state = get_hall_status();                  /*Get the current status feedback from the HALL*/
        if ((state) && (state != last_state))       /*If the previous Hall sensor state was "magnet moving away" and the current Hall sensor state is "magnet approaching"*/
            set_led_state(false);                   /*Set the Corresponding Output Level of GPIO*/
        else if ((!state) && (state != last_state)) /*If the previous Hall sensor state was "magnet approaching" and the current Hall sensor state is "magnet moving away"*/
            set_led_state(true);                    /*Set the Corresponding Output Level of GPIO*/
        last_state = state;                         /*Save the previous HALL state*/
        vTaskDelay(10 / portTICK_PERIOD_MS);        /*Delay 10ms*/
    }
#endif
#endif
}
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/