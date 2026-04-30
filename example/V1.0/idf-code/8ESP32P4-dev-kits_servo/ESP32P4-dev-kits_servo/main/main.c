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
#ifdef CONFIG_BSP_SERVO_ENABLED
    err = servo_init(); /*Servo Driver Initialization*/
    if (err != ESP_OK)
        init_fail("servo", err);
#endif
}

void app_main(void)
{
    MAIN_INFO("----------Demo version----------");
    MAIN_INFO("----------Start the test--------");
    Init();
#ifdef CONFIG_BSP_SERVO_ENABLED
    while (1)
    {
        for (uint8_t i = 0; i < 5; i++) /*Execute control with different speeds in a loop*/
        {
            set_servo_status(forward_dir, i); /*Set the servo motor direction and speed*/
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            set_servo_status(reverse_dir, i); /*Set the servo motor direction and speed*/
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
        set_servo_status(stop, 1); /*Set the servo motor stop*/
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
#endif
}
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/