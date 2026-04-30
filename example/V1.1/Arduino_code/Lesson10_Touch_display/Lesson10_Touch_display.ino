#include "Arduino.h"
#include <esp_display_panel.hpp>
#include <lvgl.h>
#include <lvgl_v8_port.h>
#include "Adafruit_NeoPixel.h"
#define LED_NUM     2   // Define the number of LEDs in the NeoPixel strip
#define LED_PIN     8   // Define the GPIO pin number for the LED indicator

Adafruit_NeoPixel strip(LED_NUM, LED_PIN, NEO_GRB + NEO_KHZ800);  // Initialize NeoPixel strip

using namespace esp_panel::drivers;
using namespace esp_panel::board;

// ESP32-P4's MIPI D-PHY requires specific voltage to function.
// LDO3 is typically routed to the MIPI power rail on P4 hardware.
esp_err_t board_p4_ldo_init() 
{
    esp_err_t err = ESP_OK;
    esp_ldo_channel_handle_t ldo3_handle = NULL;
    esp_ldo_channel_config_t ldo3_cfg = {
        .chan_id = 3,           // LDO Channel 3
        .voltage_mv = 2500,     // Set to 2500mV (2.5V)
    };

    Serial.println("Initializing LDO3 to 2.5V...");
    err = esp_ldo_acquire_channel(&ldo3_cfg, &ldo3_handle);
    if (err != ESP_OK) {
        Serial.printf("LDO3 Power Error: %s\n", esp_err_to_name(err));
        return err;
    } else {
        Serial.println("LDO3 Power enabled successfully.");
    }
    
    // --- Power Configuration (LDO4 for I2C/touch pull up) ---
    esp_ldo_channel_handle_t ldo4_handle = NULL;
    esp_ldo_channel_config_t ldo4_cfg = {
        .chan_id = 4,           // LDO Channel 4
        .voltage_mv = 3300,     // Set to 3300mV (3.3V)
    };

    Serial.println("Initializing LDO4 to 3.3V...");
    err = esp_ldo_acquire_channel(&ldo4_cfg, &ldo4_handle);
    if (err != ESP_OK) {
        Serial.printf("LDO4 Power Error: %s\n", esp_err_to_name(err));
        return err;
    } else {
        Serial.println("LDO4 Power enabled successfully.");
    }

    return ESP_OK;
}

// Button event handler function
static void button_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);              /* Get the event code of an event */
    lv_obj_t *btn = lv_event_get_target(e);                   /* Get the object originally targeted by the event */
    int led_index = (int)(intptr_t)lv_event_get_user_data(e); /* Get the user_data passed when the event was registered on the object */

    if (code == LV_EVENT_PRESSED)  // When the button is pressed
    {
        lv_obj_add_state(btn, LV_STATE_PRESSED); /* Add pressed state to the button */
        // set_single_led_status(led_index, 0xFF);  /* Light up according to the color corresponding to the key press */
        // MAIN_INFO("BTN[%d] pressed", led_index);
        switch(led_index)  // Control the LED color based on the button index
        {
            case 0:
                strip.setPixelColor(0, strip.Color(255, 0, 0)); // Red
                break;
            case 1:
                strip.setPixelColor(0, strip.Color(0, 255, 0)); // Green
                break;
            case 2:
                strip.setPixelColor(0, strip.Color(0, 0, 255)); // Blue
                break;
            case 3:
                strip.setPixelColor(1, strip.Color(0, 255, 0)); // Green (second LED)
                break;
        }

    }
    else if (code == LV_EVENT_RELEASED)  // When the button is released
    {
        lv_obj_clear_state(btn, LV_STATE_PRESSED); /* Remove pressed state from the button */
        // set_single_led_status(led_index, 0x00); /* Turn off LEDs */
        // MAIN_INFO("BTN[%d] released", led_index);
        strip.setPixelColor(0, strip.Color(0, 0, 0));  // Turn off LED 0
        strip.setPixelColor(1, strip.Color(0, 0, 0));  // Turn off LED 1
    }
    strip.show();  // Update the LED strip
}

void setup()
{
    pinMode(32, OUTPUT);  // Set GPIO 32 as output
    pinMode(33, OUTPUT);  // Set GPIO 33 as output
    digitalWrite(32, 0);  // Set GPIO 32 to low
    digitalWrite(33, 1);  // Set GPIO 33 to high
    Serial.begin(115200); // Start serial communication

    Serial.println("Initializing board");

    board_p4_ldo_init();  // Initialize the LDO power channels

    strip.begin();            // Initialize the NeoPixel strip
    strip.show();             // Initialize the LEDs
    strip.setBrightness(50);  // Set the brightness of the LEDs

    Board *board = new Board();  // Initialize the display board
    board->init();  // Initialize the display hardware

    board->begin();  // Start the display board

    lvgl_port_init(board->getLCD(), board->getTouch());  // Initialize LVGL port for touch and display
    board->getBacklight()->setBrightness(0);  // Turn off the backlight initially
    
    lv_obj_t *scr = lv_scr_act();  // Get the current screen object

    lv_obj_set_style_bg_color(scr, lv_color_black(), LV_PART_MAIN);  // Set the background color to black
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);  // Set the background opacity

    lv_obj_t *label_1 = lv_label_create(scr);  // Create a label for the text
    lv_label_set_text(label_1, "Hello P4!");  // Set the text for the label
    lv_obj_set_style_text_color(label_1, lv_color_white(), LV_STATE_DEFAULT);  // Set text color to white
    lv_obj_align(label_1, LV_ALIGN_CENTER, 0, -50);  // Center align the label
    lv_obj_set_style_text_font(label_1, &lv_font_montserrat_30, 0);  // Set the font for the label

    // Create an array of buttons
    lv_obj_t *buttons[4];
    lv_color_t btn_color[4] = {
        lv_palette_main(LV_PALETTE_RED),      // Red button color
        lv_palette_main(LV_PALETTE_YELLOW),   // Yellow button color
        lv_palette_main(LV_PALETTE_GREEN),    // Green button color
        lv_palette_main(LV_PALETTE_BLUE)      // Blue button color
    };
    const char *button_texts[] = {"RED", "YELLOW", "GREEN", "BLUE"};  // Button text labels

    for (int i = 0; i < 4; i++)
    {
        buttons[i] = lv_btn_create(scr);                                     /* Create button object */
        lv_obj_set_size(buttons[i], 100, 100);                               /* Set the size of the button */
        lv_obj_set_pos(buttons[i], (162 + (i * 200)), 300);                 /* Set the position of the button */
        lv_obj_set_style_radius(buttons[i], 15, LV_STATE_DEFAULT);           /* Set rounded corners for the button */
        
        lv_obj_t *label = lv_label_create(buttons[i]);                       /* Create a label for each button */
        lv_label_set_text(label, button_texts[i]);                            /* Set the label text */
        lv_obj_set_style_text_font(label, &lv_font_montserrat_20, LV_PART_MAIN);  /* Set the font for the label */
        lv_obj_center(label);                                                 /* Align the label to the center of the button */
        
        // Default state styles
        lv_obj_set_style_bg_color(buttons[i], btn_color[i], LV_STATE_DEFAULT);  /* Set background color of the button */
        lv_obj_set_style_text_color(buttons[i], lv_color_black(), LV_STATE_DEFAULT);  /* Set text color of the button */
        lv_obj_set_style_border_width(buttons[i], 2, LV_STATE_DEFAULT);         /* Set the border width of the button */
        lv_obj_set_style_border_color(buttons[i], lv_color_black(), LV_STATE_DEFAULT);  /* Set the border color of the button */
        
        // Pressed state styles
        lv_obj_set_style_bg_color(buttons[i], btn_color[i], LV_STATE_PRESSED);  /* Set background color when button is pressed */
        lv_obj_set_style_text_color(buttons[i], lv_color_black(), LV_STATE_PRESSED);  /* Set text color when button is pressed */
        lv_obj_set_style_border_color(buttons[i], lv_palette_main(LV_PALETTE_GREY), LV_STATE_PRESSED);  /* Set border color when button is pressed */
    
        // Add event callback for the buttons with user data for LED index
        lv_obj_add_event_cb(buttons[i], button_event_handler, LV_EVENT_ALL, (void *)(intptr_t)i);  
    }

    lvgl_port_unlock();  // Unlock the LVGL port after setup

    delay(200);  // Short delay
    board->getBacklight()->setBrightness(100);  // Set the backlight brightness to 100
}

void loop()
{
    delay(1000);  // Delay for 1 second
}