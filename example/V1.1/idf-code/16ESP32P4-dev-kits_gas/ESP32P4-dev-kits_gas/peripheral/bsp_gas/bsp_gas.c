/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "bsp_gas.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#ifdef CONFIG_BSP_GAS_ENABLED
adc_cali_handle_t gas_cali_handle = NULL;
gas_config gas_data;
#endif
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_GAS_ENABLED
esp_err_t gas_init(adc_oneshot_unit_handle_t handle)
{
    esp_err_t err = ESP_OK;
    adc_oneshot_chan_cfg_t adc_config = {
        .bitwidth = SOC_ADC_DIGI_MAX_BITWIDTH, /*ADC conversion result bits*/
        .atten = ADC_ATTEN_DB_12,              /*ADC attenuation*/
    };
    gas_data.gas_handle = handle;                                                    /*Obtain the handle of the ADC controller*/
    err = adc_oneshot_config_channel(gas_data.gas_handle, GAS_CHANNEL, &adc_config); /*Set ADC oneshot mode required configurations*/
    if (err != ESP_OK)
    {
        GAS_ERROR("create adc Doneshot channel fail");
        return err;
    }
    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,                 /*Use ADC unit 1*/
        .chan = GAS_CHANNEL,                   /*Use ADC channel 1*/
        .atten = ADC_ATTEN_DB_12,              /*ADC attenuation*/
        .bitwidth = SOC_ADC_DIGI_MAX_BITWIDTH, /*ADC conversion result bits*/
    };
    err = adc_cali_create_scheme_curve_fitting(&cali_config, &gas_cali_handle); /*Create a Curve Fitting calibration scheme*/
    if (err != ESP_OK)
    {
        GAS_ERROR("create adc scheme curve  fitting fail");
        return err;
    }
    gas_data.RL = 4.7;
    return err;
}

esp_err_t get_gas_voltage()
{
    static int vol_sum = 0;
    static uint8_t vol_reading_cnt = 0;
    esp_err_t err = ESP_OK;
    int vaule = 0;
    while (1)
    {
        err = adc_oneshot_read(gas_data.gas_handle, GAS_CHANNEL, &vaule); /*Get one ADC conversion raw result*/
        if (err != ESP_OK)
            return err;
        vol_sum += vaule; /*Accumulate the reading for averaging*/
        vol_reading_cnt++;
        if (vol_reading_cnt == 2) /*Process when we have collected 2 samples*/
        {
            err = adc_cali_raw_to_voltage(gas_cali_handle, vol_sum / vol_reading_cnt, &gas_data.gas_voltage); /*Convert ADC raw data to calibrated voltage*/
            if (err != ESP_OK)
            {
                GAS_ERROR("GAS adc voltage cali fail");
                return err;
            }
            vol_sum = 0;
            vol_reading_cnt = 0;                                       /*Reset accumulators for next reading cycle*/
            gas_data.gas_vaule = ((float)gas_data.gas_voltage) / 1000; /*Convert millivolts to volts (divide by 1000)*/
            break;                                                     /*Exit the while loop after successful processing*/
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    return err;
}

float get_gas_vaule()
{
    return gas_data.gas_vaule; /*Return the voltage value collected by the ADC*/
}

float get_r0_calibration()
{
    static uint8_t vol_reading_cnt = 0;
    static float vol_sum = 0;
    while (1)
    {
        if (get_gas_voltage() == ESP_OK) /*Attempt to read current gas sensor voltage*/
        {
            vol_sum += gas_data.gas_vaule; /*Accumulate voltage value*/
            vol_reading_cnt++;             /*Increment reading counter*/
        }
        if (vol_reading_cnt >= 10) /*Check if we have collected enough samples (10 readings)*/
        {
            /*
             * Calculate R0 using the voltage divider formula:
             * R0 = ((Vcc - Vavg) * RL) / Vavg
             * Where Vavg = vol_sum / vol_reading_cnt
             *
             * This formula derives from:
             * Rs = (Vcc - Vout) * RL / Vout
             * Where Rs is sensor resistance, and in clean air Rs = R0
             */
            gas_data.R0 = ((5 - (vol_sum / vol_reading_cnt)) * gas_data.RL) / (vol_sum / vol_reading_cnt);
            vol_reading_cnt = 0; /*Reset accumulators for potential future calibrations*/
            vol_sum = 0;
            break;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    return gas_data.R0;
}

float get_gas_data()
{
    float RS = 0;                    /*Current sensor resistance in ohms*/
    float ppm = 0;                   /*Gas concentration in Parts Per Million*/
    if (get_gas_voltage() == ESP_OK) /*Read current gas sensor voltage*/
    {
        /*
         * Calculate current sensor resistance (RS) using voltage divider formula:
         * RS = ((Vcc - Vout) * RL) / Vout
         * Where:
         * - Vcc = 5V (supply voltage)
         * - Vout = gas_data.gas_vaule (measured voltage across load resistor)
         * - RL = gas_data.RL (load resistor value in ohms)
         *
         * This formula derives from: Vout = Vcc * RL / (RS + RL)
         */
        RS = ((5 - gas_data.gas_vaule) * gas_data.RL) / gas_data.gas_vaule;
        /*
         * Convert resistance ratio to PPM using calibration curve:
         * ppm = ((11.5428 * R0) / RS)^0.6549 * 100
         * Where:
         * - R0 = gas_data.R0 (baseline resistance in clean air)
         * - RS = current sensor resistance
         *
         * This empirical formula is specific to the gas sensor type and characteristics.
         * The constants (11.5428 and 0.6549) are derived from the sensor's datasheet
         * or calibration data for a specific gas (e.g., MQ-2 for LPG, MQ-135 for air quality).
         */
        ppm = pow(((11.5428 * gas_data.R0) / RS), 0.6549f) * 100;
    }
    return ppm;
}
#endif
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/