#include "Arduino.h"  // Include the Arduino library
#include <Adafruit_NeoPixel.h>  // Include the Adafruit NeoPixel library

#define LED_NUM     2   // Define the number of LEDs in the strip (2 LEDs)
#define LED_PIN     8   // Define the GPIO pin number for the LED strip (pin 8)

Adafruit_NeoPixel strip(LED_NUM, LED_PIN, NEO_GRB + NEO_KHZ800);  // Create a NeoPixel strip object with 2 LEDs, on pin 8

void setup()
{
  strip.begin();            // Initialize the NeoPixel strip
  strip.show();             // Make sure the strip is turned off initially
  strip.setBrightness(50);  // Set the brightness of the LEDs (range: 0 to 255)
}

void loop()
{
  // Set the first LED (index 0) to white color (RGB: 255, 255, 255)
  strip.setPixelColor(0, strip.Color(255, 255, 255));   
  // Set the second LED (index 1) to white color (RGB: 255, 255, 255)
  strip.setPixelColor(1, strip.Color(255, 255, 255));   
  
  strip.show();            // Update the strip to show the new colors
  delay(1000);             // Wait for 1 second (1000 milliseconds)

  // Turn off the first LED (index 0)
  strip.setPixelColor(0, strip.Color(0, 0, 0));   
  // Turn off the second LED (index 1)
  strip.setPixelColor(1, strip.Color(0, 0, 0));   

  strip.show();            // Update the strip to show the new (off) state
  delay(1000);             // Wait for 1 second before repeating the loop
}