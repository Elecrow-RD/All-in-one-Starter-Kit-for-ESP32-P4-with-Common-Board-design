/*————————————————————————————————————————Header file declaration————————————————————————————————————————*/
#include "bsp_display.h"
/*——————————————————————————————————————Header file declaration end——————————————————————————————————————*/

/*——————————————————————————————————————————Variable declaration—————————————————————————————————————————*/
#ifdef CONFIG_BSP_DISPLAY_ENABLED
esp_lcd_panel_handle_t panel_handle = NULL;          /*Type of LCD panel handle*/
static esp_lcd_dsi_bus_handle_t mipi_dsi_bus = NULL; /*Type of MIPI DSI bus handle*/
static esp_lcd_panel_io_handle_t mipi_dbi_io = NULL; /*Type of LCD panel IO handle*/
static lv_display_t *my_lvgl_disp = NULL;            /*Backward compatibility with LVGL 8*/
#endif
/*————————————————————————————————————————Variable declaration end———————————————————————————————————————*/

/*—————————————————————————————————————————Functional function———————————————————————————————————————————*/
#ifdef CONFIG_BSP_DISPLAY_ENABLED
static esp_err_t blight_init(void)
{
    esp_err_t err = ESP_OK;
    const gpio_config_t gpio_cofig = {
        .pin_bit_mask = ((1ULL << LCD_GPIO_RESET) | (1ULL << LCD_GPIO_BLIGHT) | (1ULL << LCD_GPIO_UPDN) | (1ULL << LCD_GPIO_SHLR)), /* GPIO pin: set with bit mask, each bit maps to a GPIO */
        .mode = GPIO_MODE_OUTPUT,                                                                                                   /* GPIO mode: set input/output mode                     */
        .pull_up_en = false,                                                                                                        /* GPIO pull-up                                         */
        .pull_down_en = false,                                                                                                      /* GPIO pull-down                                       */
        .intr_type = GPIO_INTR_DISABLE,                                                                                             /* GPIO interrupt type                                  */
    };
    err = gpio_config(&gpio_cofig); /*Configure GPIO*/
    if (err != ESP_OK)
        return err;

    const ledc_timer_config_t timer_config = {
        .clk_cfg = LEDC_USE_PLL_DIV_CLK,      /* Configure LEDC source clock from ledc_clk_cfg_t*/
        .duty_resolution = LEDC_TIMER_11_BIT, /*LEDC channel duty resolution*/
        .freq_hz = BLIGHT_PWM_Hz,             /*LEDC timer frequency (Hz)*/
        .speed_mode = LEDC_LOW_SPEED_MODE,    /*LEDC speed speed_mode, high-speed mode (only exists on esp32) or low-speed mode*/
        .timer_num = LEDC_TIMER_0,            /* The timer source of channel (0 - LEDC_TIMER_MAX-1)*/
    };
    const ledc_channel_config_t channel_config = {
        .gpio_num = LCD_GPIO_BLIGHT,       /*the LEDC output gpio_num*/
        .speed_mode = LEDC_LOW_SPEED_MODE, /*LEDC speed speed_mode, high-speed mode (only exists on esp32) or low-speed mode*/
        .channel = LEDC_CHANNEL_0,         /*LEDC channel (0 - LEDC_CHANNEL_MAX-1)*/
        .intr_type = LEDC_INTR_DISABLE,    /*configure interrupt, Fade interrupt enable  or Fade interrupt disable*/
        .timer_sel = LEDC_TIMER_0,         /*Select the timer source of channel (0 - LEDC_TIMER_MAX-1)*/
        .duty = 0,                         /*EDC channel duty, the range of duty setting is [0, (2**duty_resolution)] */
        .hpoint = 0,                       /*LEDC channel hpoint value, the range is [0, (2**duty_resolution)-1]*/
    };
    err = ledc_timer_config(&timer_config); /*LEDC timer configuration Configure LEDC timer with the given source timer/frequency(Hz)/duty_resolution*/
    if (err != ESP_OK)
        return err;
    err = ledc_channel_config(&channel_config); /*LEDC channel configuration Configure LEDC channel with the given channel/output gpio_num/interrupt/source timer/frequency(Hz)/LEDC dut*/
    if (err != ESP_OK)
        return err;
    return err;
}

/*brightness -  (0 - 100) */
esp_err_t set_lcd_blight(uint32_t brightness)
{
    esp_err_t err = ESP_OK;
    if (brightness != 0)
    {
        err = ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, ((brightness * 18) + 200)); /*LEDC set duty*/
        if (err != ESP_OK)
            return err;
        err = ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0); /*LEDC update channel parameters*/
        if (err != ESP_OK)
            return err;
    }
    else
    {
        err = ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
        if (err != ESP_OK)
            return err;
        err = ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        if (err != ESP_OK)
            return err;
    }
    return err;
}

static esp_err_t display_port_init(void)
{
    esp_err_t err = ESP_OK;
    lcd_color_rgb_pixel_format_t dpi_pixel_format; /*LCD color pixel format in RGB color space*/
    /*MIPI DSI bus configuration structure*/
    esp_lcd_dsi_bus_config_t bus_config = {
        .bus_id = 0,                                 /*Select which DSI controller, index from 0*/
        .num_data_lanes = 2,                         /*Number of data lanes, if set to 0, the driver will fallback to use maximum number of lanes*/
        .phy_clk_src = MIPI_DSI_PHY_CLK_SRC_DEFAULT, /*MIPI DSI PHY clock source*/
        .lane_bit_rate_mbps = 900,                   /*Lane bit rate in Mbps*/
    };
    err = esp_lcd_new_dsi_bus(&bus_config, &mipi_dsi_bus); /*Create MIPI DSI bus handle*/
    if (err != ESP_OK)
        return err;
    /*Panel IO configuration structure, for MIPI DSI command interface*/
    esp_lcd_dbi_io_config_t dbi_config = {
        .virtual_channel = 0, /*Virtual channel ID, index from 0*/
        .lcd_cmd_bits = 8,    /*Bit-width of LCD command*/
        .lcd_param_bits = 8,  /*Bit-width of LCD parameter*/
    };
    err = esp_lcd_new_panel_io_dbi(mipi_dsi_bus, &dbi_config, &mipi_dbi_io); /*Create LCD panel IO, for MIPI DSI DBI interface*/
    if (err != ESP_OK)
        return err;
    /*Select according to the number of data display digits*/
    if (BITS_PER_PIXEL == 24)
        dpi_pixel_format = LCD_COLOR_PIXEL_FORMAT_RGB888;
    else if (BITS_PER_PIXEL == 18)
        dpi_pixel_format = LCD_COLOR_PIXEL_FORMAT_RGB666;
    else if (BITS_PER_PIXEL == 16)
        dpi_pixel_format = LCD_COLOR_PIXEL_FORMAT_RGB565;
    /*MIPI DSI DPI panel configuration structur*/
    const esp_lcd_dpi_panel_config_t dpi_config = {
        .dpi_clk_src = MIPI_DSI_DPI_CLK_SRC_DEFAULT, /*MIPI DSI DPI clock source*/
        .dpi_clock_freq_mhz = 51,                    /* DPI clock frequency in MHz*/
        .virtual_channel = 0,                        /*Virtual channel ID, index from 0*/
        .pixel_format = dpi_pixel_format,            /*Pixel format that used by the MIPI LCD device*/
        .num_fbs = 1,                                /*Number of screen-sized frame buffers that allocated by the drivered */
        .video_timing = {
            .h_size = H_size,         /*Horizontal resolution*/
            .v_size = V_size,         /*Vertical resolution*/
            .hsync_back_porch = 160,  /*Horizontal back porch, number of pixel clock between hsync and start of line active data*/
            .hsync_pulse_width = 70,  /*Horizontal sync width, in pixel cloc*/
            .hsync_front_porch = 160, /*Horizontal front porch, number of pixel clock between the end of active data and the next hsync*/
            .vsync_back_porch = 23,   /*Vertical back porch, number of invalid lines between vsync and start of frame*/
            .vsync_pulse_width = 10,  /*Vertical sync width, in number of lines*/
            .vsync_front_porch = 12,  /*Vertical front porch, number of invalid lines between the end of frame and the next vsync*/
        },
        .flags.use_dma2d = true, /*Use DMA2D to copy user buffer to the frame buffer when necessary*/
    };
    /*LCD panel vendor configuration.*/
    ek79007_vendor_config_t vendor_config = {
        .mipi_config = {
            .dsi_bus = mipi_dsi_bus,   /*MIPI-DSI bus configuration*/
            .dpi_config = &dpi_config, /*MIPI-DPI panel configuration */
        },
    };
    /*Configuration structure for panel device*/
    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_GPIO_RESET,           /* GPIO used to reset the LCD panel, set to -1 if it's not used*/
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB, /*Set RGB element order, RGB or BGR*/
        .bits_per_pixel = BITS_PER_PIXEL,           /*Color depth, in bpp*/
        .vendor_config = &vendor_config,            /*vendor specific configuration, optional, left as NULL if not used*/
    };
    err = esp_lcd_new_panel_ek79007(mipi_dbi_io, &panel_config, &panel_handle); /*Create LCD panel for model EK79007*/
    if (err != ESP_OK)
        return err;
    err = esp_lcd_panel_reset(panel_handle); /*Reset LCD panel*/
    if (err != ESP_OK)
        return err;
    err = esp_lcd_panel_init(panel_handle); /*Initialize LCD panel*/
    if (err != ESP_OK)
        return err;
    return err;
}

static void display_port_deinit(void)
{
    if (esp_lcd_panel_del(panel_handle) != ESP_OK)
        DISPLAY_ERROR("deinit panel_handle error");
    if (esp_lcd_panel_io_del(mipi_dbi_io) != ESP_OK)
        DISPLAY_ERROR("deinit mipi_dbi_io error");
    if (esp_lcd_del_dsi_bus(mipi_dsi_bus) != ESP_OK)
        DISPLAY_ERROR("deinit mipi_dsi_bus error");

    panel_handle = NULL;
    mipi_dbi_io = NULL;
    mipi_dsi_bus = NULL;

    set_lcd_blight(0);
}

static esp_err_t lvgl_init()
{
    esp_err_t err = ESP_OK;
    const lvgl_port_cfg_t lvgl_cfg = {
        .task_priority = configMAX_PRIORITIES - 4, /* LVGL task priority */
        .task_stack = 8192,                        /* LVGL task stack size */
        .task_affinity = -1,                       /* LVGL task pinned to core (-1 is no affinity) */
        .task_max_sleep_ms = 10,                   /* Maximum sleep in LVGL task */
        .timer_period_ms = 5,                      /* LVGL timer tick period in ms */
    };
    err = lvgl_port_init(&lvgl_cfg); /*Initialize LVGL portation*/
    if (err != ESP_OK)
    {
        DISPLAY_ERROR("LVGL port initialization failed");
    }

    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = mipi_dbi_io,                                      /*LCD panel IO handle*/
        .panel_handle = panel_handle,                                  /*LCD panel handle*/
        .control_handle = panel_handle,                                /*LCD panel control handle*/
        .buffer_size = (H_size * V_size * ((BITS_PER_PIXEL + 7) / 8)), /*Size of the buffer for the screen in pixels*/
        .double_buffer = true,                                         /*True, if should be allocated two buffers*/
        .hres = H_size,                                                /*LCD display horizontal resolution*/
        .vres = V_size,                                                /*LCD display vertical resolution*/
        .monochrome = false,                                           /*True, if display is monochrome and using 1bit for 1px*/
#if LVGL_VERSION_MAJOR >= 9
        .color_format = LV_COLOR_FORMAT_RGB565,
#endif
        .rotation = {
            .swap_xy = false,  /*LCD Screen swapped X and Y (in esp_lcd driver)*/
            .mirror_x = false, /*LCD Screen mirrored X (in esp_lcd driver)*/
            .mirror_y = false, /*LCD Screen mirrored Y (in esp_lcd driver)*/
        },
        .flags = {
            .buff_dma = false,   /*Allocated LVGL buffer will be DMA capable*/
            .buff_spiram = true, /*Allocated LVGL buffer will be in PSRAM*/
            .sw_rotate = true,   /* Use software rotation (slower) or PPA if available*/
#if LVGL_VERSION_MAJOR >= 9
            .swap_bytes = true, /*Swap bytes in RGB656 (16-bit) before send to LCD driver*/
#endif
#if CONFIG_DISPLAY_LVGL_FULL_REFRESH
            .full_refresh = true,
#else
            .full_refresh = false, /*Always make the whole screen redrawn*/
#endif
#if CONFIG_DISPLAY_LVGL_DIRECT_MODE
            .direct_mode = true,
#else
            .direct_mode = false, /*Use screen-sized buffers and draw to absolute coordinates*/
#endif
        },
    };
    const lvgl_port_display_dsi_cfg_t lvgl_dpi_cfg = {
        .flags = {
#if CONFIG_DISPLAY_LVGL_AVOID_TEAR
            .avoid_tearing = true,
#else
            /*Use internal MIPI-DSI buffers as a LVGL draw buffers to avoid tearing effect, enabling this option requires over two LCD buffers and may reduce the frame rate*/
            .avoid_tearing = false,
#endif
        },
    };
    my_lvgl_disp = lvgl_port_add_disp_dsi(&disp_cfg, &lvgl_dpi_cfg); /*Add MIPI-DSI display handling to LVGL*/
    if (my_lvgl_disp == NULL)
    {
        err = ESP_FAIL;
        DISPLAY_ERROR("LVGL dsi port add fail");
    }
    return err;
}

esp_err_t display_init()
{
    esp_err_t err = ESP_OK;
    err = blight_init(); /*Backlight initialization function*/
    if (err != ESP_OK)
        return err;
    err = display_port_init(); /*Display screen interface initialization function*/
    if (err != ESP_OK)
        return err;
    err = lvgl_init(); /*Screen interface registration LVGL function*/
    if (err != ESP_OK)
    {
        DISPLAY_ERROR("Display init fail");
        return err;
    }
    gpio_set_level(LCD_GPIO_UPDN, 0); /*Set the display screen vertical mirroring*/
    gpio_set_level(LCD_GPIO_SHLR, 1); /*Set the display screen horizontal mirroring*/
    set_lcd_blight(0);                /*Set the backlight to turn off*/
    return err;
}

esp_err_t get_display_buff(void **buff)
{
    esp_err_t err = ESP_OK;
    err = esp_lcd_dpi_panel_get_frame_buffer(panel_handle, 1, buff);
    return err;
}

#endif
/*———————————————————————————————————————Functional function end—————————————————————————————————————————*/