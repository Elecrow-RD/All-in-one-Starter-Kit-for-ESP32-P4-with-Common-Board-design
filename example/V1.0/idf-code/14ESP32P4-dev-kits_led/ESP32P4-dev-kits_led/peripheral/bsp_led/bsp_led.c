/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "bsp_led.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#ifdef CONFIG_BSP_LED_ENABLED
static rmt_channel_handle_t led_chan = NULL;
static rmt_encoder_handle_t led_encoder = NULL;
static uint8_t led_strip_pixels[4];
#endif
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_LED_ENABLED
/*The layout of RMT symbol stored in memory*/
static const rmt_symbol_word_t signal_zero = {
    .level0 = 1,    /*Level of the first part*/
    .duration0 = 4, /*Duration of level0*/
    .level1 = 0,    /*Level of the second part*/
    .duration1 = 9, /*Duration of level1*/
};

static const rmt_symbol_word_t signal_one = {
    .level0 = 1,    /*Level of the first part*/
    .duration0 = 6, /*Duration of level0*/
    .level1 = 0,    /*Level of the second part*/
    .duration1 = 7, /*Duration of level1*/
};

static const rmt_symbol_word_t signal_reset = {
    .level0 = 0,       /*Level of the first part*/
    .duration0 = 1500, /*Duration of level0*/
    .level1 = 0,       /*Level of the second part*/
    .duration1 = 1500, /*Duration of level1*/
};
/*Callback to call for encoding data into RMT items*/
static size_t encoder_callback(const void *data, size_t data_size, size_t symbols_written, size_t symbols_free, rmt_symbol_word_t *symbols, bool *done, void *arg)
{
    /* Check if there's enough space in the symbols buffer for a full byte (8 symbols)*/
    if (symbols_free < 8) /*The maximum amount of symbols that can be written into the `symbols` buffer*/
    {
        return 0; /*Not enough space, return 0 to indicate no symbols were written*/
    }
    /*Calculate current position in the data stream based on symbols already written*/
    /*Each byte requires 8 symbols, so divide by 8 to get the byte position*/
    size_t data_pos = symbols_written / 8; /*Current position in encoded stream, in symbols*/
    uint8_t *data_bytes = (uint8_t *)data; /*Cast void pointer to byte array for easier access */
    if (data_pos < data_size)              /*Check if we still have data bytes to encode*/
    {
        size_t symbol_pos = 0; /*Position counter within the current symbols buffer*/
        for (int bitmask = 0x80; bitmask != 0; bitmask >>= 1)
        {
            /*Encode each bit of the current byte (MSB first)*/
            /*Bitmask starts at 0x80 (10000000 binary) and shifts right each iteration*/
            if (data_bytes[data_pos] & bitmask) /*Check if the current bit is set (1) or clear (0)*/
            {
                symbols[symbol_pos++] = signal_one; /*use the symbol defined for logic high*/
            }
            else
            {
                symbols[symbol_pos++] = signal_zero; /*use the symbol defined for logic low*/
            }
        }
        return symbol_pos;
    }
    else
    {
        symbols[0] = signal_reset; /*All data bytes have been encoded, send reset symbol to indicate end of transmission*/
        *done = 1;                 /*Write reset symbol to mark transmission end*/
        return 1;
    }
}

esp_err_t led_init()
{
    esp_err_t err = ESP_OK;
    rmt_tx_channel_config_t tx_chan_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,               /*Clock source of RMT TX channel*/
        .gpio_num = LED_GPIO,                         /*GPIO number used by RMT TX channel*/
        .mem_block_symbols = 64,                      /*The size of the memory block bytes(64 * 4 = 256)*/
        .resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ, /*10 MHz tick resolution(1 tick = 0.1 µs)*/
        .trans_queue_depth = 4,                       /*max num*/
    };
    err = rmt_new_tx_channel(&tx_chan_config, &led_chan); /*Create a RMT TX channel*/
    if (err != ESP_OK)
    {
        LED_ERROR("create new rmt tx channel fail");
        return err;
    }
    const rmt_simple_encoder_config_t simple_encoder_cfg = {
        .callback = encoder_callback, /*Callback to call for encoding data into RMT items*/
    };
    err = rmt_new_simple_encoder(&simple_encoder_cfg, &led_encoder); /*Create RMT simple callback encoder*/
    if (err != ESP_OK)
    {
        LED_ERROR("create new rmt simple encoder fail");
        return err;
    }
    err = rmt_enable(led_chan); /*Enable the RMT channel*/
    if (err != ESP_OK)
    {
        LED_ERROR("enable rmt fail");
        return err;
    }
    return err;
}

esp_err_t set_single_led_status(led_byte_order_t color, uint8_t brightness)
{
    esp_err_t err = ESP_OK;
    /*Configure RMT transmission for single shot operation (no repetition)*/
    rmt_transmit_config_t tx_config = {
        .loop_count = 0,
    };
    /*Set the specified color component to the desired brightness and turn off all other color components*/
    switch (color)
    {
    case LED_BYTE_RED:
        led_strip_pixels[0] = 0x00;       /*Yellow component off*/
        led_strip_pixels[1] = brightness; /*Red component on*/
        led_strip_pixels[2] = 0x00;       /*Green component off*/
        led_strip_pixels[3] = 0x00;       /*Blue component off*/
        break;
    case LED_BYTE_GREEN:
        led_strip_pixels[0] = 0x00;
        led_strip_pixels[1] = 0x00;
        led_strip_pixels[2] = brightness;
        led_strip_pixels[3] = 0x00;
        break;
    case LED_BYTE_BLUE:
        led_strip_pixels[0] = 0x00;
        led_strip_pixels[1] = 0x00;
        led_strip_pixels[2] = 0x00;
        led_strip_pixels[3] = brightness;
        break;
    case LED_BYTE_YELLOW:
        led_strip_pixels[0] = brightness;
        led_strip_pixels[1] = 0x00;
        led_strip_pixels[2] = 0x00;
        led_strip_pixels[3] = 0x00;
        break;
    default:
        break;
    }
    err = rmt_transmit(led_chan, led_encoder, led_strip_pixels, sizeof(led_strip_pixels), &tx_config); /*Transmit data by RMT TX channel*/
    if (err != ESP_OK)
    {
        LED_INFO("transmit rmt cmd fail");
        return err;
    }
    err = rmt_tx_wait_all_done(led_chan, portMAX_DELAY); /*Wait for all pending TX transactions done*/
    if (err != ESP_OK)
    {
        LED_INFO("transmit rmt fail");
        return err;
    }
    return err;
}

esp_err_t set_led_status(uint32_t bit)
{
    esp_err_t err = ESP_OK;
    rmt_transmit_config_t tx_config = {
        .loop_count = 0,
    };
    led_strip_pixels[0] = (uint8_t)(bit >> 16);                                                        /*Extract Yellow component (bits 23-16)*/
    led_strip_pixels[1] = (uint8_t)(bit >> 24);                                                        /*Extract Red component (bits 31-24)*/
    led_strip_pixels[2] = (uint8_t)(bit >> 8);                                                         /*Extract Green component (bits 15-8)*/
    led_strip_pixels[3] = (uint8_t)(bit >> 0);                                                         /*Extract Blue component (bits 7-0)*/
    err = rmt_transmit(led_chan, led_encoder, led_strip_pixels, sizeof(led_strip_pixels), &tx_config); /*Transmit data by RMT TX channel*/
    if (err != ESP_OK)
    {
        LED_INFO("transmit rmt cmd fail");
        return err;
    }
    err = rmt_tx_wait_all_done(led_chan, portMAX_DELAY); /*Wait for all pending TX transactions done*/
    if (err != ESP_OK)
    {
        LED_INFO("transmit rmt fail");
        return err;
    }
    return err;
}

esp_err_t led_waterfall()
{
    esp_err_t err = ESP_OK;
    uint8_t red = 0xFF;
    uint8_t yellow = 0xFF;
    uint8_t green = 0xFF;
    uint8_t blue = 0xFF;
    uint32_t data = 0;
    err = set_led_status(0xFFFFFFFF);
    if (err != ESP_OK)
        return err;
    for (uint8_t j = 0; j < 255; j++)
    {
        red--;
        yellow--;
        green--;
        blue--;
        data = yellow << 24 | red << 16 | green << 8 | blue << 0;
        err = set_led_status(data);
        vTaskDelay(20 / portTICK_PERIOD_MS);
        if (err != ESP_OK)
            return err;
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
    err = set_led_status(0xFF000000);
    if (err != ESP_OK)
        return err;
    data = 0xFF000000;
    for (uint8_t i = 0; i < 4; i++)
    {
        err = set_led_status(data >> (i * 8));
        vTaskDelay(500 / portTICK_PERIOD_MS);
        if (err != ESP_OK)
            return err;
    }
    err = set_led_status(0x00000000);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    if (err != ESP_OK)
        return err;
    return err;
}
#endif
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/