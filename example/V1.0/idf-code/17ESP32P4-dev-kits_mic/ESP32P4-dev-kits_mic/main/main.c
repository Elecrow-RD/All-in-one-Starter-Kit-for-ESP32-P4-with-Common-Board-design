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
#ifdef CONFIG_BSP_SD_ENABLED
    err = sd_init(); /*SD Initialization*/
    if (err != ESP_OK)
        init_fail("sd", err);
    vTaskDelay(500 / portTICK_PERIOD_MS);
#endif
#ifdef CONFIG_BSP_MIC_ENABLED
    err = mic_init(); /*Microphone Initialization*/
    if (err != ESP_OK)
        init_fail("mic", err);
    vTaskDelay(500 / portTICK_PERIOD_MS);
#endif
}

void app_main(void)
{
    MAIN_INFO("----------Demo version----------");
    MAIN_INFO("----------Start the test--------");
    Init();
#ifdef CONFIG_BSP_MIC_ENABLED
#ifdef CONFIG_BSP_SD_ENABLED
    size_t Recorded_size = 0;
    mic_readwav_to_sd("/sdcard/test.wav", 5, &Recorded_size); /*Microphone recording saved to SD card(5s)*/
    MAIN_INFO("Recorded %d bytes", Recorded_size);
#endif
#endif
}
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/