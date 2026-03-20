#include "Arduino.h"
#include <esp_display_panel.hpp>
#include <lvgl.h>
#include <lvgl_v8_port.h>
#include "bsp_i2c.h"
#include "bsp_bh1750.h"

// Namespaces for easier access to panel driver and board functionality
using namespace esp_panel::drivers;
using namespace esp_panel::board;

// Task handle for reading BH1750 sensor data
TaskHandle_t read_bh1750;

// Label to display the light level on the screen
lv_obj_t *label_1 = NULL;

// Function to update the label with the current lux value from the BH1750 sensor
void update_bh1750_value(float lux)
{
    if (label_1)
    {
        char buffer[60];
        snprintf(buffer, sizeof(buffer), "lux = %.1f", lux); // Format the data into a string
        lv_label_set_text(label_1, buffer);                   // Set the formatted string as label text
    }
}

// Task that continuously reads data from the BH1750 sensor and updates the display
void bh1750_read_task(void *param)
{
    float lux = 0;
    while (1)
    {
        lux = bh1750_read_data(); // Read the illuminance data from the BH1750 sensor
        if (lux == (-1))
        {
            if (lvgl_port_lock(0))
            {
                lv_label_set_text(label_1, "bh1750 read data error"); // Display error message if reading fails
                lvgl_port_unlock();
            }
        }
        else
        {
            // If data is valid, update the label with the new lux value
            if (lvgl_port_lock(0))
            {
                update_bh1750_value(lux); // Update the BH1750 data on the screen
                lvgl_port_unlock();
            }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1 second before reading again
    }
}

// Setup function to initialize the board and peripherals
void setup()
{
    pinMode(32, OUTPUT); // Set pin 32 as an output
    pinMode(33, OUTPUT); // Set pin 33 as an output
    digitalWrite(32, 0); // Set pin 32 low
    digitalWrite(33, 1); // Set pin 33 high

    i2c_init();           // Initialize I2C communication
    bh1750_begin();       // Initialize the BH1750 sensor
    Serial.begin(115200); // Initialize serial communication for debugging

    Serial.println("Initializing board");

    // Initialize the board object and configure it
    Board *board = new Board();
    board->init();
    board->begin();

    // Initialize LVGL graphics library with the board's display
    lvgl_port_init(board->getLCD(), nullptr);
    board->getBacklight()->setBrightness(0); // Set initial brightness to 0

    // Get the current screen object
    lv_obj_t *scr = lv_scr_act();

    // Set background color to black for the screen
    lv_obj_set_style_bg_color(scr, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);

    // Lock LVGL port before modifying the UI
    lvgl_port_lock(-1);

    // Create a label to display the lux value
    label_1 = lv_label_create(scr);
    lv_label_set_text(label_1, "lux = 0.0"); // Initial text for the label
    lv_obj_set_style_text_color(label_1, lv_color_white(), LV_STATE_DEFAULT); // Set text color to white
    lv_obj_set_style_text_font(label_1, &lv_font_montserrat_30, 0); // Set font size
    lv_obj_align(label_1, LV_ALIGN_CENTER, 0, -20); // Align label in the center of the screen

    // Unlock LVGL port after modifying the UI
    lvgl_port_unlock();

    // Create a task to periodically read data from the BH1750 sensor
    xTaskCreate(bh1750_read_task, "read_bh1750", 4096, NULL, configMAX_PRIORITIES - 5, &read_bh1750);
    delay(200);

    // Set the backlight brightness to 100 (full brightness)
    board->getBacklight()->setBrightness(100); 
}

// Main loop (empty in this case)
void loop()
{
#if 0
    lvgl_port_lock(-1); // Lock LVGL port (commented out code)

    lvgl_port_unlock(); // Unlock LVGL port

    delay(500); // Delay for 500 milliseconds
#endif
}