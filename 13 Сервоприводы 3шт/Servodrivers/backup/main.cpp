// =================== НАСТРОЙКИ ===================

// аналоговый пин для подключения датчика
#define ANALOG_INPUT_PIN A5

//
#define ANALOG_VALUE 512

#define SERVO1_MIN_ANGLE 0
#define SERVO1_MAX_ANGLE 180

#define SERVO2_MIN_ANGLE 0
#define SERVO2_MAX_ANGLE 180

#define SERVO3_MIN_ANGLE 0
#define SERVO3_MAX_ANGLE 180

// три входных пина для трехпозиционного переключателя
#define BTN1_PIN 3
#define BTN2_PIN 4
#define BTN3_PIN 5

// пины для подключения сервоприводов
#define SERVO1_PIN 9
#define SERVO2_PIN 10
#define SERVO3_PIN 11

// скорость сервоприводов (градусов/сек) [Может быть не всегда точно, т.к есть погрешность]
#define SERVO1_SPEED 180
#define SERVO2_SPEED 180
#define SERVO3_SPEED 180

// плавное ускорение сервоприводов. если значение 1, то без ускорения, сразу максимальная скорость
#define SERVO1_ACCEL 1
#define SERVO2_ACCEL 1
#define SERVO3_ACCEL 1

// начальный угол сервопривода (угол, который будет задаваться после загрузки устройства)
#define SERVO1_START_ANGLE 0
#define SERVO2_START_ANGLE 0
#define SERVO3_START_ANGLE 0

// =================================================

#include <Arduino.h>

#include <ServoDriverSmooth.h>
ServoDriverSmooth servo1, servo2, servo3;

#include <EncButton.h>
EncButton<EB_TICK, BTN1_PIN, INPUT_PULLUP> btn1;
EncButton<EB_TICK, BTN2_PIN, INPUT_PULLUP> btn2;
EncButton<EB_TICK, BTN3_PIN, INPUT_PULLUP> btn3;

#include "GyverWDT.h"

void setup()
{
  Watchdog.enable(INTERRUPT_MODE, WDT_PRESCALER_64);

  servo1.attach(SERVO1_PIN);
  servo1.setSpeed(SERVO1_SPEED);
  servo1.setAccel(SERVO1_ACCEL);
  servo1.setTargetDeg(SERVO1_START_ANGLE);

  servo2.attach(SERVO2_PIN);
  servo2.setSpeed(SERVO2_SPEED);
  servo2.setAccel(SERVO2_ACCEL);
  servo2.setTargetDeg(SERVO2_START_ANGLE);

  servo3.attach(SERVO3_PIN);
  servo3.setSpeed(SERVO3_SPEED);
  servo3.setAccel(SERVO3_ACCEL);
  servo3.setTargetDeg(SERVO3_START_ANGLE);

  Serial.begin(9600);
}

byte cnt2 = 0;
byte cnt3 = 0;

ISR(WATCHDOG)
{
  Serial.println("Hello from INDUS!");
}

void loop()
{
  servo1.tick();
  servo2.tick();
  servo3.tick();

  btn1.tick();
  btn2.tick();
  btn3.tick();

  Serial.print("Сервопривод 1: ");
  Serial.print(servo1.getCurrentDeg());
  Serial.print("    Сервопривод 2: ");
  Serial.print(servo2.getCurrentDeg());
  Serial.print("    Сервопривод 3: ");
  Serial.println(servo3.getCurrentDeg());
  Serial.println();

  Watchdog.reset();

  if (btn1.hold())
  {
    Serial.println("BTN1 HOLD");
    if (analogRead(ANALOG_INPUT_PIN) <= ANALOG_VALUE)
    {
      servo1.setTargetDeg(SERVO1_MIN_ANGLE);
    }
    else if (digitalRead(ANALOG_INPUT_PIN) >= ANALOG_VALUE)
    {
      servo1.setTargetDeg(SERVO1_MAX_ANGLE);
    }
  }

  // else if (btn2.hold())
  // {
  //   if (cnt2 == 0)
  //   {
  //     if (analogRead(ANALOG_INPUT_PIN) <= ANALOG_VALUE)
  //     {
  //       servo1.setTargetDeg(SERVO1_MIN_ANGLE);
  //       cnt2++;
  //     }
  //     if (digitalRead(ANALOG_INPUT_PIN) >= ANALOG_VALUE)
  //     {
  //       servo1.setTargetDeg(SERVO1_MAX_ANGLE);
  //       cnt2++;
  //     }
  //   }

  //   else if (cnt2 == 2)
  //   {
  //     if (analogRead(ANALOG_INPUT_PIN) <= ANALOG_VALUE)
  //     {
  //       servo2.setTargetDeg(SERVO2_MIN_ANGLE);
  //       cnt2++;
  //     }
  //     else if (digitalRead(ANALOG_INPUT_PIN) >= ANALOG_VALUE)
  //     {
  //       servo2.setTargetDeg(SERVO2_MAX_ANGLE);
  //       cnt2++;
  //     }
  //   }

  //   if (cnt2 == 4)
  //     cnt2 = 0;
  // }

  // else if (btn3.hold())
  // {
  //   if (cnt3 == 0)
  //   {
  //     if (analogRead(ANALOG_INPUT_PIN) <= ANALOG_VALUE)
  //     {
  //       servo1.setTargetDeg(SERVO1_MIN_ANGLE);
  //       cnt3++;
  //     }
  //     if (digitalRead(ANALOG_INPUT_PIN) >= ANALOG_VALUE)
  //     {
  //       servo1.setTargetDeg(SERVO1_MAX_ANGLE);
  //       cnt3++;
  //     }
  //   }

  //   else if (cnt3 == 2)
  //   {
  //     if (analogRead(ANALOG_INPUT_PIN) <= ANALOG_VALUE)
  //     {
  //       servo2.setTargetDeg(SERVO2_MIN_ANGLE);
  //       cnt3++;
  //     }
  //     else if (digitalRead(ANALOG_INPUT_PIN) >= ANALOG_VALUE)
  //     {
  //       servo2.setTargetDeg(SERVO2_MAX_ANGLE);
  //       cnt3++;
  //     }
  //   }

  //   else if (cnt3 == 4)
  //   {
  //     if (analogRead(ANALOG_INPUT_PIN) <= ANALOG_VALUE)
  //     {
  //       servo3.setTargetDeg(SERVO3_MIN_ANGLE);
  //       cnt3++;
  //     }
  //     else if (digitalRead(ANALOG_INPUT_PIN) >= ANALOG_VALUE)
  //     {
  //       servo3.setTargetDeg(SERVO3_MAX_ANGLE);
  //       cnt3++;
  //     }
  //   }

  //   if (cnt3 == 6)
  //     cnt3 == 0;
  // }
}
