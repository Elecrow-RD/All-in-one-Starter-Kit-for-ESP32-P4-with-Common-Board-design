#include "Arduino.h"

#define RELAY_PIN 42 // Define the GPIO pin number for controlling the relay (pin 42)

void setup()
{
    pinMode(RELAY_PIN, OUTPUT);  // Set the relay control pin as an output
    digitalWrite(RELAY_PIN, LOW); // Initialize the relay to OFF (LOW)
}

void loop()
{
    digitalWrite(RELAY_PIN, HIGH); // Turn the relay ON
    delay(5000);                   // Wait for 5000 milliseconds (5 seconds)

    digitalWrite(RELAY_PIN, LOW);  // Turn the relay OFF
    delay(5000);                   // Wait for 5000 milliseconds (5 seconds)
}