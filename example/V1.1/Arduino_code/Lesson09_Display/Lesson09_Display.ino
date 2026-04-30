#include "Arduino.h"
#include <esp_display_panel.hpp>
#include <lvgl.h>
#include <lvgl_v8_port.h>

// Use namespaces from the ESP display panel library
using namespace esp_panel::drivers;
using namespace esp_panel::board;

void setup()
{
    // Configure GPIO pins 32 and 33 as output pins
    pinMode(32, OUTPUT);
    pinMode(33, OUTPUT);

    // Set initial pin states
    digitalWrite(32, 0);
    digitalWrite(33, 1);

    // Start serial communication for debugging
    Serial.begin(115200);
    
    Serial.println("Initializing board");

    // Create a new Board object which handles display, touch, and backlight
    Board *board = new Board();

    // Initialize board hardware
    board->init();
    
    // Start the board (initialize peripherals like LCD, touch, etc.)
    board->begin();

    // Initialize LVGL graphics library with LCD and touch drivers
    lvgl_port_init(board->getLCD(), board->getTouch()); 

    // Turn off backlight initially (brightness = 0)
    board->getBacklight()->setBrightness(0); 

    // Get the currently active LVGL screen object
    lv_obj_t *scr = lv_scr_act();

    // Set screen background color to black
    lv_obj_set_style_bg_color(scr, lv_color_black(), LV_PART_MAIN);

    // Make the background fully opaque
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN); 

    // Lock LVGL to safely modify UI elements
    lvgl_port_lock(-1);

    // Create a label object on the screen
    lv_obj_t *label_1 = lv_label_create(scr);

    // Set the text displayed on the label
    lv_label_set_text(label_1, "Hello P4!");

    // Set label text color to white
    lv_obj_set_style_text_color(label_1, lv_color_white(), LV_STATE_DEFAULT);

    // Set label font size to Montserrat 30
    lv_obj_set_style_text_font(label_1, &lv_font_montserrat_30, 0);

    // Align the label to the center of the screen, slightly upward (-20 pixels)
    lv_obj_align(label_1, LV_ALIGN_CENTER, 0, -20);

    // Unlock LVGL after UI updates are done
    lvgl_port_unlock();

    // Small delay to ensure UI setup is stable
    delay(200);

    // Turn on the backlight to full brightness (100%)
    board->getBacklight()->setBrightness(100); 
}

void loop()
{
    // Main loop does nothing for now, just waits
    delay(1000);
}