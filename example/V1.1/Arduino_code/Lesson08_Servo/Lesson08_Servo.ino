#include "Arduino.h"
#include <ESP32Servo.h>

Servo myservo; 

int pos = 0;

void setup()
{
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    
	myservo.attach(25, 500, 2500);

}

void loop()
{
	myservo.write(0);	
	delay(2000); 
	myservo.write(90);
	delay(2000); 
	myservo.write(180);
	delay(2000); 
	myservo.write(90);
	delay(2000); 
}