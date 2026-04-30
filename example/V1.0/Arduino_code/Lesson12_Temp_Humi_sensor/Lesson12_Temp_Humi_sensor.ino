#include "Arduino.h"
#include <esp_display_panel.hpp>
#include <lvgl.h>
#include <lvgl_v8_port.h>
#include "bsp_i2c.h"
#include "bsp_dht20.h"

using namespace esp_panel::drivers;
using namespace esp_panel::board;

// Declare task handle for DHT20 reading
TaskHandle_t read_dht20;
// Declare a label for displaying temperature and humidity
lv_obj_t *label_1 = NULL;

// Function to update the DHT20 values on the display
void update_dht20_value(float temperature, float humidity)
{
    if (label_1)  // If label is created
    {
        char buffer[60];
        snprintf(buffer, sizeof(buffer), "Temperature = %.1f C  Humidity = %.1f %%", temperature, humidity); // Format the data into a string
        lv_label_set_text(label_1, buffer);  // Set the new text for the label
    }
}

// Task to read the DHT20 sensor data
void dht20_read_task(void *param)
{
    static dht20_data_t measurements;  // Store the measurement data
    while (1)
    {
        if (dht20_is_calibrated() == ESP_OK)  // Check if the DHT20 sensor is calibrated
        {
            Serial.println("is calibrated....");
        }
        else
        {
            Serial.println("is NOT calibrated....");
            if (dht20_begin() != ESP_OK)  // Reinitialize the DHT20 sensor if not calibrated
            {
                Serial.println("dht20 init again false");
                vTaskDelay(100 / portTICK_PERIOD_MS);  // Wait for a bit before retrying
                continue;
            }
        }
        
        if (dht20_read_data(&measurements) != ESP_OK)  // Read the temperature and humidity data from the DHT20 sensor
        {
            if (lvgl_port_lock(0))
            {
                lv_label_set_text(label_1, "dht20 read data error");  // Display an error message if reading fails
                lvgl_port_unlock();
            }
            Serial.println("dht20 read data error");
        }
        else  // If reading is successful
        {
            if (lvgl_port_lock(0))
            {
                update_dht20_value(measurements.temperature, measurements.humidity);  // Update the displayed values on the screen
                lvgl_port_unlock();
            }
            Serial.printf("Temperature:\t%.1fC\n", measurements.temperature);  // Print temperature to Serial Monitor
            Serial.printf("Humidity:   \t%.1f%%", measurements.humidity);  // Print humidity to Serial Monitor
        }
        
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // Wait for 1 second before reading again
    }
}

void setup()
{
    // Set pins for control (e.g., for backlight)
    pinMode(32, OUTPUT);
    pinMode(33, OUTPUT);
    digitalWrite(32, 0);  // Turn off pin 32
    digitalWrite(33, 1);  // Turn on pin 33

    i2c_init();  // Initialize I2C for communication
    dht20_begin();  // Initialize the DHT20 sensor
    Serial.begin(115200);  // Start Serial communication

    Serial.println("Initializing board");

    // Initialize the display board
    Board *board = new Board();
    board->init();
    board->begin();

    // Initialize the LVGL port with the display
    lvgl_port_init(board->getLCD(), nullptr); 
    board->getBacklight()->setBrightness(0);  // Turn off the backlight initially
    lv_obj_t *scr = lv_scr_act();  // Get the current screen

    // Set screen background color to black
    lv_obj_set_style_bg_color(scr, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    
    lvgl_port_lock(-1);  // Lock LVGL port before creating the label
    label_1 = lv_label_create(scr);  // Create a label for displaying data
    lv_label_set_text(label_1, "Temperature = 0.0 C  Humidity = 0.0 %");  // Set initial text
    lv_obj_set_style_text_color(label_1, lv_color_white(), LV_STATE_DEFAULT);  // Set text color to white
    lv_obj_set_style_text_font(label_1, &lv_font_montserrat_30, 0);  // Set font for text
    lv_obj_align(label_1, LV_ALIGN_CENTER, 0, -20);  // Align label to center of screen
    lvgl_port_unlock();  // Unlock LVGL port after creating label

    // Create a task to handle DHT20 readings in the background
    xTaskCreate(dht20_read_task, "read_dht20", 4096, NULL, configMAX_PRIORITIES - 5, &read_dht20);
    
    delay(200);  // Delay to ensure initialization is complete
    board->getBacklight()->setBrightness(100);  // Set backlight brightness to 100
}

void loop()
{
    // The loop function is left empty since we handle everything in the DHT20 task
#if 0
    lvgl_port_lock(-1);

    lvgl_port_unlock(); 

    delay(500);
#endif
}