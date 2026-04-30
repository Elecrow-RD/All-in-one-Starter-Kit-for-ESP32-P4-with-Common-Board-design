/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "bsp_lsm6ds3tr.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#ifdef CONFIG_BSP_LSM6DS3_SENSOR_ENABLED
lsm6ds3tr my_lsm6ds3;
static i2c_master_dev_handle_t lsm6ds3_handle = NULL;
#endif
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_LSM6DS3_SENSOR_ENABLED

static esp_err_t lsm6ds3_getchipID(void)
{
    esp_err_t err = ESP_OK;
    uint8_t buf = 0;
    err = i2c_read_reg(lsm6ds3_handle, WHO_AM_I_ADDRESS, &buf, 1); /*I2C Read read-only register*/
    if (err != ESP_OK)
        return err;
    if (buf != 0x6A) /*Check if the id is 0x6A*/
    {
        err = ESP_FAIL;
        return err;
    }
    return err;
}

esp_err_t lsm6ds3_reset(void)
{
    esp_err_t err = ESP_OK;
    uint8_t buf[1] = {0};
    buf[0] = 0x80;
    err = i2c_write_reg(lsm6ds3_handle, CTRL3_C_ADDRESS, buf[0]); /*Reboot memory content*/
    if (err != ESP_OK)
        return err;
    vTaskDelay(15 / portTICK_PERIOD_MS);
    err = i2c_read_reg(lsm6ds3_handle, CTRL3_C_ADDRESS, buf, 1); /*I2C Read register status*/
    if (err != ESP_OK)
        return err;
    buf[0] |= 0x01;
    err = i2c_write_reg(lsm6ds3_handle, CTRL3_C_ADDRESS, buf[0]); /*Software reset*/
    if (err != ESP_OK)
        return err;
    while (buf[0] & 0x01) /*(0: normal mode; 1: reset device)*/
    {
        i2c_read_reg(lsm6ds3_handle, CTRL3_C_ADDRESS, buf, 1); /*I2C Read register status*/
        vTaskDelay(15 / portTICK_PERIOD_MS);
    }
    return err;
}

esp_err_t lsm6ds3_set_BDU(bool state)
{
    esp_err_t err = ESP_OK;
    uint8_t buf[1] = {0};
    err = i2c_read_reg(lsm6ds3_handle, CTRL3_C_ADDRESS, buf, 1); /*I2C Read register status*/
    if (err != ESP_OK)
        return err;
    if (state == true)
    {
        buf[0] |= 0x40;
        err = i2c_write_reg(lsm6ds3_handle, CTRL3_C_ADDRESS, buf[0]); /*1: output registers not updated until MSB and LSB have been read*/
        if (err != ESP_OK)
            return err;
    }
    else
    {
        buf[0] &= 0xBF;
        err = i2c_write_reg(lsm6ds3_handle, CTRL3_C_ADDRESS, buf[0]); /*0: continuous update*/
        if (err != ESP_OK)
            return err;
    }

    err = i2c_read_reg(lsm6ds3_handle, CTRL3_C_ADDRESS, buf, 1); /*I2C Read register status*/
    if (err != ESP_OK)
        return err;

    return err;
}

esp_err_t lsm6ds3_set_acc_rate(uint8_t rate)
{
    esp_err_t err = ESP_OK;
    uint8_t buf[1] = {0};
    err = i2c_read_reg(lsm6ds3_handle, CTRL1_XL_ADDRESS, buf, 1); /*I2C Read register status*/
    if (err != ESP_OK)
        return err;
    buf[0] |= rate;
    err = i2c_write_reg(lsm6ds3_handle, CTRL1_XL_ADDRESS, buf[0]); /*Accelerometer Output data rate*/
    if (err != ESP_OK)
        return err;
    return err;
}

esp_err_t lsm6ds3_set_acc_fullscale(uint8_t value)
{
    esp_err_t err = ESP_OK;
    uint8_t buf[1] = {0};
    err = i2c_read_reg(lsm6ds3_handle, CTRL1_XL_ADDRESS, buf, 1);
    if (err != ESP_OK)
        return err;
    buf[0] |= value;
    err = i2c_write_reg(lsm6ds3_handle, CTRL1_XL_ADDRESS, buf[0]); /*Accelerometer full-scale selection*/
    if (err != ESP_OK)
        return err;
    return err;
}

esp_err_t lsm6ds3_set_gyr_rate(uint8_t rate)
{
    esp_err_t err = ESP_OK;
    uint8_t buf[1] = {0};
    err = i2c_read_reg(lsm6ds3_handle, CTRL2_G_ADDRESS, buf, 1);
    if (err != ESP_OK)
        return err;
    buf[0] |= rate;
    err = i2c_write_reg(lsm6ds3_handle, CTRL2_G_ADDRESS, buf[0]); /*Gyroscope Output data rate*/
    if (err != ESP_OK)
        return err;
    return err;
}

esp_err_t lsm6ds3_set_gyr_fullscale(uint8_t value)
{
    esp_err_t err = ESP_OK;
    uint8_t buf[1] = {0};
    err = i2c_read_reg(lsm6ds3_handle, CTRL2_G_ADDRESS, buf, 1);
    if (err != ESP_OK)
        return err;
    buf[0] |= value;
    err = i2c_write_reg(lsm6ds3_handle, CTRL2_G_ADDRESS, buf[0]); /*Gyroscope full-scale selection*/
    if (err != ESP_OK)
        return err;
    return err;
}

esp_err_t lsm6ds3_set_acc_bandwidth(uint8_t BW0XL, uint8_t ODR)
{
    esp_err_t err = ESP_OK;
    uint8_t buf[1] = {0};
    err = i2c_read_reg(lsm6ds3_handle, CTRL1_XL_ADDRESS, buf, 1);
    if (err != ESP_OK)
        return err;
    buf[0] |= BW0XL;
    err = i2c_write_reg(lsm6ds3_handle, CTRL1_XL_ADDRESS, buf[0]); /*Anti-aliasing filter bandwidth selection*/
    if (err != ESP_OK)
        return err;

    err = i2c_read_reg(lsm6ds3_handle, CTRL8_XL_ADDRESS, buf, 1);
    if (err != ESP_OK)
        return err;
    buf[0] |= ODR;
    err = i2c_write_reg(lsm6ds3_handle, CTRL8_XL_ADDRESS, buf[0]); /*Accelerometer ODR register setting*/
    if (err != ESP_OK)
        return err;
    return err;
}

esp_err_t lsm6ds3_set_gry_register4(uint8_t cmd)
{
    esp_err_t err = ESP_OK;
    uint8_t buf[1] = {0};

    err = i2c_read_reg(lsm6ds3_handle, CTRL4_C_ADDRESS, buf, 1);
    if (err != ESP_OK)
        return err;
    buf[0] |= cmd;
    err = i2c_write_reg(lsm6ds3_handle, CTRL4_C_ADDRESS, buf[0]); /*Enable gyroscope digital LPF1 (0: disabled; 1: enabled)*/
    if (err != ESP_OK)
        return err;
    return err;
}

esp_err_t lsm6ds3_set_gry_register6(uint8_t cmd)
{
    esp_err_t err = ESP_OK;
    uint8_t buf[1] = {0};
    err = i2c_read_reg(lsm6ds3_handle, CTRL6_C_ADDRESS, buf, 1);
    if (err != ESP_OK)
        return err;
    buf[0] |= cmd;
    err = i2c_write_reg(lsm6ds3_handle, CTRL6_C_ADDRESS, buf[0]); /*this bit must be set to ‘0’ for the correct operation of the device.*/
    if (err != ESP_OK)
        return err;
    return err;
}

esp_err_t lsm6ds3_set_gry_register7(uint8_t cmd)
{
    esp_err_t err = ESP_OK;
    uint8_t buf[1] = {0};
    err = i2c_read_reg(lsm6ds3_handle, CTRL7_G_ADDRESS, buf, 1);
    if (err != ESP_OK)
        return err;
    buf[0] |= cmd;
    err = i2c_write_reg(lsm6ds3_handle, CTRL7_G_ADDRESS, buf[0]); /*Gyroscope digital high-pass filter enable and Gyroscope digital HP filter cutoff selection 260MHZ*/
    if (err != ESP_OK)
        return err;
    return err;
}

static uint8_t lsm6ds3_get_status(void)
{
    uint8_t buf[1] = {0};
    i2c_read_reg(lsm6ds3_handle, STATUS_REG_ADDRESS, buf, 1); /*I2C Read register status*/
    return buf[0];
}

/*I2C reads the registers, performing a cyclic reading process*/
static esp_err_t lsm6ds3_data_read(uint8_t reg_addr, uint8_t *rev_data, uint8_t length)
{
    esp_err_t err = ESP_OK;
    while (length) /*Read the length*/
    {
        err = i2c_read_reg(lsm6ds3_handle, reg_addr++, rev_data++, 1); /*I2C Read register data*/
        if (err != ESP_OK)
            return err;
        length--;
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    return err;
}

esp_err_t lsm6ds3_get_acc(uint8_t fsxl)
{
    esp_err_t err = ESP_OK;
    uint8_t buf[6];
    int16_t acc[3];
    err = lsm6ds3_data_read(OUTX_L_XL_ADDRESS, buf, 6); /*Read the value stored in the accelerometer register*/
    if (err != ESP_OK)
        return err;
    else
    {
        acc[0] = buf[1] << 8 | buf[0];
        acc[1] = buf[3] << 8 | buf[2];
        acc[2] = buf[5] << 8 | buf[4];
        switch (fsxl) /*Based on setting different full-scale parameters, convert the parameters returned by the accelerometer*/
        {
        case LSM6DS3TRC_ACC_FSXL_2G:
            my_lsm6ds3.acc_x = ((float)acc[0] * 0.061f) / 1000.0f;
            my_lsm6ds3.acc_y = ((float)acc[1] * 0.061f) / 1000.0f;
            my_lsm6ds3.acc_z = ((float)acc[2] * 0.061f) / 1000.0f;
            break;
        case LSM6DS3TRC_ACC_FSXL_4G:
            my_lsm6ds3.acc_x = ((float)acc[0] * 0.122f) / 1000.0f;
            my_lsm6ds3.acc_y = ((float)acc[1] * 0.122f) / 1000.0f;
            my_lsm6ds3.acc_z = ((float)acc[2] * 0.122f) / 1000.0f;
            break;
        case LSM6DS3TRC_ACC_FSXL_8G:
            my_lsm6ds3.acc_x = ((float)acc[0] * 0.244f) / 1000.0f;
            my_lsm6ds3.acc_y = ((float)acc[1] * 0.244f) / 1000.0f;
            my_lsm6ds3.acc_z = ((float)acc[2] * 0.244f) / 1000.0f;
            break;
        case LSM6DS3TRC_ACC_FSXL_16G:
            my_lsm6ds3.acc_x = ((float)acc[0] * 0.488f) / 1000.0f;
            my_lsm6ds3.acc_y = ((float)acc[1] * 0.488f) / 1000.0f;
            my_lsm6ds3.acc_z = ((float)acc[2] * 0.488f) / 1000.0f;
            break;
        }
    }
    return err;
}

esp_err_t lsm6ds3_get_gry(uint8_t fsg)
{
    esp_err_t err = ESP_OK;
    uint8_t buf[6];
    int16_t gry[3];
    err = lsm6ds3_data_read(OUTX_L_G_ADDRESS, buf, 6); /*Read the values stored in the gyroscope register*/
    if (err != ESP_OK)
        return err;
    else
    {
        gry[0] = buf[1] << 8 | buf[0];
        gry[1] = buf[3] << 8 | buf[2];
        gry[2] = buf[5] << 8 | buf[4];
        switch (fsg) /*Based on setting different full-scale parameters, convert the parameters returned by the gyroscope*/
        {
        case LSM6DS3TRC_GYR_FSG_245:
            my_lsm6ds3.gry_x = ((float)gry[0] * 8.750f) / 1000.0f;
            my_lsm6ds3.gry_y = ((float)gry[1] * 8.750f) / 1000.0f;
            my_lsm6ds3.gry_z = ((float)gry[2] * 8.750f) / 1000.0f;
            break;
        case LSM6DS3TRC_GYR_FSG_500:
            my_lsm6ds3.gry_x = ((float)gry[0] * 17.50f) / 1000.0f;
            my_lsm6ds3.gry_y = ((float)gry[1] * 17.50f) / 1000.0f;
            my_lsm6ds3.gry_z = ((float)gry[2] * 17.50f) / 1000.0f;
            break;
        case LSM6DS3TRC_GYR_FSG_1000:
            my_lsm6ds3.gry_x = ((float)gry[0] * 35.00f) / 1000.0f;
            my_lsm6ds3.gry_y = ((float)gry[1] * 35.00f) / 1000.0f;
            my_lsm6ds3.gry_z = ((float)gry[2] * 35.00f) / 1000.0f;
            break;
        case LSM6DS3TRC_GYR_FSG_2000:
            my_lsm6ds3.gry_x = ((float)gry[0] * 70.00f) / 1000.0f;
            my_lsm6ds3.gry_y = ((float)gry[1] * 70.00f) / 1000.0f;
            my_lsm6ds3.gry_z = ((float)gry[2] * 70.00f) / 1000.0f;
            break;
        }
    }
    return err;
}

esp_err_t lsm6ds3_scan()
{
    esp_err_t err = ESP_FAIL;
    uint8_t status;
    status = lsm6ds3_get_status();                /*I2C Read register status*/
    if (status & LSM6DS3TRC_STATUS_ACCELEROMETER) /*Accelerometer new data available status*/
    {
        err = lsm6ds3_get_acc(LSM6DS3TRC_ACC_FSXL_2G);
        if (err != ESP_OK)
            return err;
    }

    if (status & LSM6DS3TRC_STATUS_GYROSCOPE) /*Gyroscope new data available status*/
    {
        err = lsm6ds3_get_gry(LSM6DS3TRC_GYR_FSG_2000);
        if (err != ESP_OK)
            return err;
    }

    return err;
}

esp_err_t lsm6ds3_begin(void)
{
    esp_err_t err = ESP_OK;
    lsm6ds3_handle = i2c_dev_register(LSM6DS3_I2C_ADDRESS); /*lsm6ds3 sensor address registration*/
    if (lsm6ds3_handle != NULL)
    {
        err = lsm6ds3_getchipID(); /*lsm6ds3 obtains ID information*/
        if (err != ESP_OK)
            return err;
        err = lsm6ds3_reset(); /*Reset the sensor of lsm6ds3*/
        if (err != ESP_OK)
            return err;
        err = lsm6ds3_set_BDU(true); /*Set BDU for lsm6ds3*/
        if (err != ESP_OK)
            return err;
        err = lsm6ds3_set_acc_rate(LSM6DS3TRC_RATE_12HZ5); /*Set the rate of the accelerometer for lsm6ds3*/
        if (err != ESP_OK)
            return err;
        err = lsm6ds3_set_gyr_rate(LSM6DS3TRC_RATE_12HZ5); /*Set the rate of the gyroscope for lsm6ds3*/
        if (err != ESP_OK)
            return err;
        err = lsm6ds3_set_acc_fullscale(LSM6DS3TRC_ACC_FSXL_2G); /*Set the full-scale of the accelerometer for lsm6ds3*/
        if (err != ESP_OK)
            return err;
        err = lsm6ds3_set_gyr_fullscale(LSM6DS3TRC_GYR_FSG_2000); /*Set the full-scale of the gyroscope for lsm6ds3*/
        if (err != ESP_OK)
            return err;
        err = lsm6ds3_set_acc_bandwidth(LSM6DS3TRC_ACC_BW0XL_400HZ, LSM6DS3TRC_ACC_LOW_PASS_ODR_100); /*Set the bandwidth of the accelerometer for lsm6ds3*/
        if (err != ESP_OK)
            return err;
        err = lsm6ds3_set_gry_register4(LSM6DS3TRC_CTRL4_LPF1_SELG_ENABLE); /*Set the gyroscope register 4 of lsm6ds3*/
        if (err != ESP_OK)
            return err;
        err = lsm6ds3_set_gry_register6(LSM6DS3TRC_CTRL6_C_FTYPE_1); /*Set the gyroscope register 6 of lsm6ds3*/
        if (err != ESP_OK)
            return err;
        err = lsm6ds3_set_gry_register7(LSM6DS3TRC_CTRL7_G_HP_EN_ENABLE | LSM6DS3TRC_CTRL7_G_HPM_260MHZ); /*Set the gyroscope register 7 of lsm6ds3*/
        if (err != ESP_OK)
            return err;
        err = lsm6ds3_getchipID(); /*lsm6ds3 obtains ID information*/
        if (err != ESP_OK)
            return err;
    }
    else
    {
        err = ESP_FAIL;
        return err;
    }
    return err;
}

#endif
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/