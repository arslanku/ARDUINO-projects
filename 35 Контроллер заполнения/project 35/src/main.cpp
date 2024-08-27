// ==================================================================

#define BTN_PIN 3
#define POT1_PIN A2
#define POT2_PIN A3
#define SERVO_PIN 5
#define SENSOR_PIN 4

#define MIN_SERVO_DELAY_TIME 5000
#define MAX_SERVO_DELAY_TIME 15000

#define MIN_SERVO_ANGLE 30
#define MAX_SERVO_ANGLE 180

// ==================================================================

#include <Arduino.h>
#include <Wire.h>

#include <Servo.h>
Servo servo;
// #include <ServoSmooth.h>
// ServoSmooth servo;

#include <EncButton.h>
Button btn(BTN_PIN);

// ==================================================================

void setup()
{
  Serial.begin(9600);

  pinMode(A0, OUTPUT);
  pinMode(A4, OUTPUT);
  digitalWrite(A0, 0);
  digitalWrite(A4, 1);

  pinMode(A1, OUTPUT);
  pinMode(A5, OUTPUT);
  digitalWrite(A1, 0);
  digitalWrite(A5, 1);

  btn.setHoldTimeout(1000);

  servo.attach(SERVO_PIN);
  // servo.setSpeed(300);
  // servo.setTargetDeg(0);
  // servo.setAccel(0.5);
  servo.write(0);
}

bool is_working_now = false;
int pot1_value, pot2_value, servo_angle;
uint32_t servo_delay_time, timer;

void loop()
{
  btn.tick();
  // servo.tick();

  // if (!to_start_position)
  // {
  //   servo.setTargetDeg(0);
  //   to_start_position = true;
  //   servo.setAccel(0);
  // }

  if (btn.click() && !is_working_now)
  {
    pot1_value = analogRead(POT1_PIN);
    pot2_value = analogRead(POT2_PIN);
    servo_angle = map(pot1_value, 0, 1023, 30, 180);
    servo_delay_time = map(pot2_value, 0, 1023, MIN_SERVO_DELAY_TIME, MAX_SERVO_DELAY_TIME);

    servo.write(servo_angle);
    // servo.setTargetDeg(servo_angle);
    is_working_now = true;
    timer = millis();
  }
  else if (btn.click() && is_working_now)
  {
    servo.write(0);
    // servo.setTargetDeg(0);
    is_working_now = false;
    timer = 0;
  }

  if ((millis() - timer >= servo_delay_time) && is_working_now)
  {
    servo.write(0);
    // servo.setTargetDeg(0);
    is_working_now = false;
    timer = 0;
  }
}
