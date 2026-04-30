#include "Arduino.h"
#include <esp_display_panel.hpp>
#include <lvgl.h>
#include <lvgl_v8_port.h>

#include <HCSR04.h>

// Create a buffer to hold the formatted distance string
char buffer[32];

// Define the trigger and echo pins for the ultrasonic sensor
const byte triggerPin = 13;
const byte echoPin = 12;

// Initialize the ultrasonic distance sensor
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);

using namespace esp_panel::drivers;
using namespace esp_panel::board;

// Create a label object for displaying the distance on the screen
lv_obj_t *label_1 = NULL;

void setup()
{
    // Set up the backlight pins for the display
    pinMode(32, OUTPUT);
    pinMode(33, OUTPUT);
    digitalWrite(32, 0); // Set pin 32 low (turn off backlight)
    digitalWrite(33, 1); // Set pin 33 high (turn on backlight)

    // Initialize serial communication at 115200 baud rate
    Serial.begin(115200);
    
    // Print a message to indicate the board is initializing
    Serial.println("Initializing board");

    // Create a Board object and initialize it
    Board *board = new Board();
    board->init();
    board->begin();

    // Initialize the LVGL (LittlevGL) library for display and touch
    lvgl_port_init(board->getLCD(), board->getTouch()); 
    
    // Turn off the backlight initially
    board->getBacklight()->setBrightness(0); 
    
    // Get the active screen (LVGL screen)
    lv_obj_t *scr = lv_scr_act();

    // Set the background color of the screen to black
    lv_obj_set_style_bg_color(scr, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN); 
    
    // Lock LVGL port (to prevent concurrent access)
    lvgl_port_lock(-1);

    // Create a label on the screen to display the distance
    label_1 = lv_label_create(scr);
    lv_label_set_text(label_1, "distance = 0.0 cm"); // Default text for the label
    lv_obj_set_style_text_color(label_1, lv_color_white(), LV_STATE_DEFAULT); // Set label text color to white
    lv_obj_set_style_text_font(label_1, &lv_font_montserrat_30, 0); // Set font size
    lv_obj_align(label_1, LV_ALIGN_CENTER, 0, -20); // Position the label in the center of the screen

    // Unlock LVGL port (allowing access to other tasks)
    lvgl_port_unlock();

    // Delay to allow the display to initialize properly
    delay(200);

    // Set the backlight brightness to a higher value (100)
    board->getBacklight()->setBrightness(100); 
}

void loop()
{
    // Measure the distance using the ultrasonic sensor (in cm)
    float distance = distanceSensor.measureDistanceCm();

    // Lock LVGL port to avoid concurrent access
    lvgl_port_lock(-1);

    // Check if the distance exceeds the threshold (35 cm)
    if(distance > 35)
    {
        // If the distance exceeds the limit, display a warning message
        lv_label_set_text(label_1, "the distance exceeds the limit");
    }
    else
    {
        // Format the measured distance into a string and update the label text
        snprintf(buffer, sizeof(buffer), "distance = %.1f cm", distance);
        lv_label_set_text(label_1, buffer);
    }

    // Unlock LVGL port after updating the display
    lvgl_port_unlock(); 

    // Print the measured distance to the serial monitor
    Serial.println(distance);

    // Add a delay of 500ms before the next loop iteration
    delay(500);
}