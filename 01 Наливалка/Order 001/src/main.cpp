#include <Arduino.h>
#include <Wire.h>
#include "ServoSmooth.h"
#include "EncButton.h"
#include "GyverTM1637.h"

#define SERVO_PIN A5

#define TRIGGER_PIN 4
#define BTN_PIN 5

#define CLK 6
#define DIO 7

#define ENC_PIN1 8
#define ENC_PIN2 9
#define ENC_PIN3 10

#define PUMP_PIN A0

#define TL_PIN_RED A2
#define TL_PIN_YEL A3
#define TL_PIN_GR A4

ServoSmooth servo1;
EncButton<EB_TICK, ENC_PIN1, ENC_PIN2, ENC_PIN3> enc; // энкодер
EncButton<EB_TICK, BTN_PIN> btn;                      // кнопка
EncButton<EB_TICK, TRIGGER_PIN> trigger;              // триггер
GyverTM1637 disp(CLK, DIO);

byte mode = 0; // 0 - автоматический, 1 - ручной

void setup()
{
  Serial.begin(9600);

  pinMode(PUMP_PIN, OUTPUT);
  pinMode(TL_PIN_RED, OUTPUT);
  pinMode(TL_PIN_YEL, OUTPUT);
  pinMode(TL_PIN_GR, OUTPUT);

  digitalWrite(TL_PIN_GR, 1);

  servo1.attach(SERVO_PIN);
  servo1.setSpeed(180);
  servo1.setAccel(0.8);
  servo1.setTargetDeg(0);

  disp.brightness(7);
  disp.clear();
  disp.displayByte(_A, _U, _t, _O);
}

void yield()
{
  servo1.tick();
}

void loop()
{
  btn.tick();
  enc.tick();
  servo1.tick();
  trigger.tick();

  static uint32_t time = 0;
  static bool enabled = false;
  static int angle = 0;

  if (btn.click()) // смена режимов
  {
    if (mode == 0)
    {
      Serial.print("Change mode 0 -> 1");

      mode = 1;
      disp.displayByte(_h, _A, _n, _d);
    }
    else if (mode == 1)
    {
      Serial.print("Change mode 1 -> 0");

      mode = 0;
      disp.displayByte(_A, _U, _t, _O);
    }
  }

  if (mode == 0 && trigger.held()) // auto
  {
    Serial.println("Auto mode started");

    digitalWrite(TL_PIN_RED, 1);
    servo1.setTargetDeg(100);
    delay(500);

    digitalWrite(TL_PIN_RED, 0);
    digitalWrite(TL_PIN_YEL, 1);

    digitalWrite(PUMP_PIN, 1);

    time = millis();
    enabled = 1;
  }

  if (enabled && millis() - time >= 60000)
  {
    digitalWrite(PUMP_PIN, 0);
    servo1.setTargetDeg(0);
    delay(500);

    digitalWrite(TL_PIN_YEL, 0);
    digitalWrite(TL_PIN_GR, 1);

    enabled = 0;
  }

  if (mode == 1 && trigger.hold()) // ручной
  {
    // servo1.tick();
    // Serial.println("Hand mode started");

    digitalWrite(TL_PIN_RED, 1);
    // servo1.setTargetDeg(0);
    digitalWrite(TL_PIN_RED, 0);
    digitalWrite(TL_PIN_YEL, 1);

    if (enc.right() && angle < 180)
    {
      Serial.println(angle);
      // servo1.write(angle);
      servo1.setTargetDeg(angle);
      angle += 10;
    }

    if (enc.left() && angle > 0)
    {
      Serial.println(angle);
      // servo1.write(angle);
      servo1.setTargetDeg(angle);
      angle -= 10;
    }

    if (enc.held()){
      time = millis();
    }

    if (enc.hold())
    {
      digitalWrite(PUMP_PIN, 1);
      // disp.clear();
      disp.displayInt((millis() - time)/ 1000 * 8);
    }
    else
      digitalWrite(PUMP_PIN, 0);
  }

  if (trigger.hold() != 1)
  {
    digitalWrite(PUMP_PIN, 0);
    servo1.setTargetDeg(0);
    digitalWrite(TL_PIN_YEL, 0);
  }
}