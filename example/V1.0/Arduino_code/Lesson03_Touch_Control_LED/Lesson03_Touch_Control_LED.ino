#include "Arduino.h"

#define TOUCH_PIN   2   // Define the GPIO pin number for the touch sensor (pin 2)
#define LED_PIN     5   // Define the GPIO pin number for the LED indicator (pin 5)

void setup()
{
    pinMode(TOUCH_PIN, INPUT);  // Set the touch sensor pin as an input
    pinMode(LED_PIN, OUTPUT);   // Set the LED pin as an output
}

void loop()
{
    // If the touch sensor is triggered (state is LOW), turn on the LED
    if (digitalRead(TOUCH_PIN) == 0)
    {
        digitalWrite(LED_PIN, HIGH);  // Turn on the LED
    }
    // If the touch sensor is not triggered (state is HIGH), turn off the LED
    else 
    {
        digitalWrite(LED_PIN, LOW);   // Turn off the LED
    }
}