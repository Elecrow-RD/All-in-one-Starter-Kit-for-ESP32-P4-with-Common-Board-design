#include "Arduino.h"

#define PIR_PIN 24 // Define the GPIO pin number for the PIR sensor (pin 24)
#define LED_PIN 5  // Define the GPIO pin number for the LED indicator (pin 5)

void setup()
{
    pinMode(PIR_PIN, INPUT);  // Set the PIR sensor pin as an input
    pinMode(LED_PIN, OUTPUT); // Set the LED pin as an output
}

void loop()
{
    // If the PIR sensor detects motion (PIR_PIN is HIGH), turn off the LED
    if(digitalRead(PIR_PIN) == 1)
    {
        digitalWrite(LED_PIN, LOW);  // Turn off the LED
    }
    // If the PIR sensor does not detect motion (PIR_PIN is LOW), turn on the LED
    else if (digitalRead(PIR_PIN) == 0)
    {
        digitalWrite(LED_PIN, HIGH); // Turn on the LED
    }
}