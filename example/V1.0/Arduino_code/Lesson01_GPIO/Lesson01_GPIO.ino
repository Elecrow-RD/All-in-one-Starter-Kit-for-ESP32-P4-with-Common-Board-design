#include "Arduino.h"  // Include the Arduino library

#define LED_GPIO 5  // Define the GPIO pin number for the LED (pin 5)

void setup() {
    pinMode(LED_GPIO, OUTPUT);  // Set the LED pin as an output
}

void loop() {
    digitalWrite(LED_GPIO, HIGH);  // Turn the LED on (set pin HIGH)
    delay(500);  // Wait for 500 milliseconds (0.5 second)

    digitalWrite(LED_GPIO, LOW);  // Turn the LED off (set pin LOW)
    delay(500);  // Wait for 500 milliseconds (0.5 second)
}