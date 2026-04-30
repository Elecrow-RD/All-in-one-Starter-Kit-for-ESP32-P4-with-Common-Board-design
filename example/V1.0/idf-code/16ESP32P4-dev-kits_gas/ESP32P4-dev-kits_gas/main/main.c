/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "main.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#ifdef CONFIG_BSP_GAS_ENABLED
TaskHandle_t gas;
#endif
static esp_ldo_channel_handle_t ldo4 = NULL;
static esp_ldo_channel_handle_t ldo3 = NULL;
adc_oneshot_unit_handle_t adc_handle = NULL;
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_GAS_ENABLED
#ifdef CONFIG_BSP_LED_ENABLED
void gas_task(void *param)
{
    float ppm_data = 0;                   /*Current gas concentration in PPM*/
    float last_ppm_data = get_gas_data(); /*Previous gas concentration reading*/
    static bool state = false;            /*LED blink state (true=on, false=off)*/
    uint32_t now_time = 0;                /*Current time in milliseconds*/
    uint32_t last_time = 0;               /*Last LED toggle time for blinking*/
    while (1)
    {
        now_time = esp_timer_get_time() / 1000;         /*Get current time in milliseconds from system timer*/
        MAIN_INFO("gas_vaule = %.2f", get_gas_vaule()); /*Log raw gas sensor voltage value for debugging*/
        ppm_data = get_gas_data();                      /*Read current gas concentration in PPM*/
        if (ppm_data >= 1000)                           /*Alarm condition: Gas concentration exceeds 1000 PPM threshold*/
        {
            if (now_time >= last_time)
            {
                if (now_time - last_time >= 500) /*Check if 500ms has elapsed since last LED toggle for blinking effect*/
                {
                    state = !state; /*Toggle LED state*/
                    if (state)
                        set_led_status(0xFFFFFFFF); /*Turn LED on*/
                    else
                        set_led_status(0x00000000); /*Turn LED off*/
                    last_time = now_time;           /*Update last toggle time*/
                }
            }
            else
            {
                last_time = now_time; /*Handle timer overflow case (unlikely in normal operation)*/
            }
        }
        else if ((ppm_data < 1000) && (last_ppm_data >= 1000)) /*Alarm cancellation condition: Concentration drops below threshold after being above it*/
        {
            set_led_status(0x00000000); /*Turn off LED when gas concentration returns to safe levels*/
        }
        last_ppm_data = ppm_data; /*Store current reading for next iteration comparison*/
        MAIN_INFO("ppm = %.2f", ppm_data);
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}
#endif
#endif

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
    adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id = ADC_UNIT_1,
    };
    err = adc_oneshot_new_unit(&init_cfg, &adc_handle);
    if (err != ESP_OK)
        init_fail("new adc oneshot", err);
#ifdef CONFIG_BSP_LED_ENABLED
    err = led_init(); /*RMT LED Initialization*/
    if (err != ESP_OK)
        init_fail("led", err);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    set_led_status(0x00000000); /*All the LEDs are off*/
#endif
#ifdef CONFIG_BSP_GAS_ENABLED
    err = gas_init(adc_handle); /*GAS ADC Initialization*/
    if (err != ESP_OK)
        init_fail("gas", err);
    float r0 = 0;
    r0 = get_r0_calibration(); /*Obtain the R0 value (load resistance data under clean air conditions) through data conversion*/
    MAIN_INFO("r0 = %.2f K", r0);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
#endif
}

void app_main(void)
{
    MAIN_INFO("----------Demo version----------");
    MAIN_INFO("----------Start the test----------");
    Init();
#ifdef CONFIG_BSP_GAS_ENABLED
#ifdef CONFIG_BSP_LED_ENABLED
    xTaskCreate(gas_task, "gas", 4096, NULL, configMAX_PRIORITIES - 5, &gas); /*Create a thread for smoke alarm recognition and processing*/
#endif
#endif
}
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/