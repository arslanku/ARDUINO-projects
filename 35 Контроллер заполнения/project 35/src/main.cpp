#include <Arduino.h>
#include <Wire.h>

// #include <ServoSmooth.h>
// ServoSmooth servo;
#include <Servo.h>
Servo servo;

#define POT_PIN A2
#define SERVO_PIN 6
#define SENSOR_PIN 4

void setup()
{
  Serial.begin(9600);
  pinMode(A0, OUTPUT);
  pinMode(A4, OUTPUT);
  digitalWrite(A0, 0);
  digitalWrite(A4, 1);

  servo.attach(SERVO_PIN);
  // servo.setSpeed(90);
  // servo.setAccel(0.2);
}

int pot_value, angle;
void loop()
{
  // servo.tick();

  pot_value = analogRead(A2);
  angle = map(pot_value, 0, 1024, 0, 180);

  if (digitalRead(SENSOR_PIN))
  {
    // servo.setTargetDeg(angle);
    servo.write(angle);
  }
  else
  {
    // servo.setTargetDeg(0);
    servo.write(0);
  }

  Serial.println(pot_value);
  delay(10);
}