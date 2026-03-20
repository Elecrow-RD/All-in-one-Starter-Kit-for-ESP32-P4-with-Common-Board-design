#include "Arduino.h"

#define HALL_PIN   7   // Define the GPIO pin number for the hall sensor (pin 7)
#define LED_PIN    5   // Define the GPIO pin number for the LED indicator (pin 5)

void setup()
{
    pinMode(HALL_PIN, INPUT);   // Set the hall sensor pin as an input
    pinMode(LED_PIN, OUTPUT);   // Set the LED pin as an output
    digitalWrite(LED_PIN, LOW); // Initialize the LED to OFF (LOW)
}

void loop()
{
    // If the hall sensor detects a magnetic field (state is HIGH), turn on the LED
    if(digitalRead(HALL_PIN) == 1) 
    {
        digitalWrite(LED_PIN, HIGH);  // Turn on the LED
    } 
    // If the hall sensor does not detect a magnetic field (state is LOW), turn off the LED
    else if (digitalRead(HALL_PIN) == 0) 
    {
        digitalWrite(LED_PIN, LOW);   // Turn off the LED
    }
}