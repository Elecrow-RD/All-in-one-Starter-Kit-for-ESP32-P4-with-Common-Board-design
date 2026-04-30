#include "Arduino.h"
#define LED_NUM     2    // Define the number of LEDs in the NeoPixel strip (2 LEDs)
#define LED_PIN     8    // Define the GPIO pin number for the LED strip (pin 8)

#include <Adafruit_NeoPixel.h>  // Include the Adafruit NeoPixel library

Adafruit_NeoPixel strip(LED_NUM, LED_PIN, NEO_GRB + NEO_KHZ800);  // Create a NeoPixel strip object with 2 LEDs on pin 8

void setup()
{
  Serial.begin(115200);            // Initialize serial communication at 115200 baud rate
  strip.begin();                   // Initialize the NeoPixel strip
  strip.show();                    // Ensure the strip is turned off initially
  strip.setBrightness(50);         // Set the brightness of the LEDs (range: 0 to 255)
  analogSetPinAttenuation(16, ADC_11db); // Set the ADC attenuation for the pin 16 to 11db (for a wider input voltage range)
}

void loop()
{
    uint32_t voltage_mv = analogReadMilliVolts(16);  // Read the voltage from pin 16 in millivolts
    float voltage_v = voltage_mv / 1000.0;           // Convert the voltage to volts

    // If the voltage is between 2.7V and 3.3V, set the first LED to red color
    if(voltage_v > 2.7 && voltage_v < 3.3)
    {
      strip.setPixelColor(0, strip.Color(255, 0, 0));  // Set the first LED to red
      Serial.println("1");
    }
    // If the voltage is less than 2.0V, set the first LED to green color
    else if(voltage_v < 2)
    {
      strip.setPixelColor(0, strip.Color(0, 255, 0));  // Set the first LED to green
      Serial.println("2");
    }
    // If the voltage is between 2.4V and 2.7V, set the first LED to blue color
    else if(voltage_v > 2.4 && voltage_v < 2.7)
    {
      strip.setPixelColor(0, strip.Color(0, 0, 255));  // Set the first LED to blue
      Serial.println("3");
    }
    // If the voltage is between 2.0V and 2.4V, set the second LED to white color
    else if (voltage_v > 2 && voltage_v < 2.4)
    {
      strip.setPixelColor(1, strip.Color(255, 255, 255));  // Set the second LED to white
      Serial.println("4");
    }
    else
    {
      // If the voltage is outside the expected range, turn off both LEDs
      strip.setPixelColor(0, strip.Color(0, 0, 0));  // Turn off the first LED
      strip.setPixelColor(1, strip.Color(0, 0, 0));  // Turn off the second LED
    }

  strip.show();  // Update the LED strip with the new colors
  delay(200);    // Wait for 200 milliseconds before repeating the loop
}