#include <Arduino.h>
#include <Wire.h>

#include <Servo.h>
Servo servo;

#include <EncButton.h>
Button btn(3);

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

  servo.attach(5);
  servo.write(0);
}

uint32_t servo_delay_time, timer;
bool is_working_now = false;
int pot1_value, pot2_value, servo_angle;

void loop()
{
  btn.tick();

  if (btn.click() && !is_working_now)
  {
    pot1_value = analogRead(2);
    pot2_value = analogRead(3);
    servo_angle = map(pot1_value, 0, 1023, 30, 180);
    servo_delay_time = map(pot2_value, 0, 1023, 10000, 50000);

    for (int pos = 0; pos <= servo_angle; pos += 1)
    {
      servo.write(pos);
      delay(10);
    }
    is_working_now = true;
    timer = millis();
  }
  else if (btn.click() && is_working_now)
  {
    for (int pos = servo_angle; pos >= 0; pos -= 1)
    {
      servo.write(pos);
      delay(10);
    }
    is_working_now = false;
    timer = 0;
  }

  if ((millis() - timer >= servo_delay_time) && is_working_now)
  {
    for (int pos = servo_angle; pos >= 0; pos -= 1)
    {
      servo.write(pos);
      delay(10);
    }
    is_working_now = false;
    timer = 0;
  }
}
