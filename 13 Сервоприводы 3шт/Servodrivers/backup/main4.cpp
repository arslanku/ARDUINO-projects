// ========================= НАСТРОЙКИ =========================

// аналоговый пин для подключения датчика
#define ANALOG_INPUT_PIN A5

// значение определенного уровня напряжения согласно 10-битному АЦП (0 - 0 вольт, 1024 - 5 вольт)
#define ANALOG_VALUE 512 // сейчас стоит значение 512, то есть примерно 2.5 вольта

// углы которые будут устанавливаться при низком (MIN) уровне напряжения и высоком (MAX)
// для каждого сервопривода отдельно
#define SERVO1_MIN_ANGLE 60
#define SERVO1_MAX_ANGLE 180

#define SERVO2_MIN_ANGLE 60
#define SERVO2_MAX_ANGLE 120

#define SERVO3_MIN_ANGLE 60
#define SERVO3_MAX_ANGLE 120

// три входных пина для трехпозиционного переключателя
// на каждый их пинов будет поступать сигнал земли, в зависимости от положения трехпозиционного переключателя
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
#define SERVO1_ACCEL 0.9
#define SERVO2_ACCEL 0.9
#define SERVO3_ACCEL 0.9

// начальный угол сервопривода (угол, который будет задаваться после загрузки устройства)
#define SERVO1_START_ANGLE 60
#define SERVO2_START_ANGLE 60
#define SERVO3_START_ANGLE 60

// =============================================================

#include <Arduino.h>

#include <ServoSmooth.h>
ServoSmooth servo[3];

#include <EncButton.h>
EncButton<EB_TICK, BTN1_PIN> btn1;
EncButton<EB_TICK, BTN2_PIN> btn2;
EncButton<EB_TICK, BTN3_PIN> btn3;

#include "GyverWDT.h"

void setup()
{
  Watchdog.enable(RESET_MODE, WDT_PRESCALER_128);

  servo[1].attach(SERVO1_PIN, 600, 2400);  // 600 и 2400 - длины импульсов
  // servo1.setSpeed(180);
  // servo1.setAccel(0.9);
  servo[1].setAutoDetach(false);
  servo[1].write(SERVO1_MIN_ANGLE);

  servo[2].attach(SERVO2_PIN, 600, 2400);  // 600 и 2400 - длины импульсов
  // servo2.setSpeed(SERVO2_SPEED);
  // servo2.setAccel(SERVO2_ACCEL);
  servo[2].setAutoDetach(0);
  servo[2].write(SERVO2_MIN_ANGLE);

  servo[3].attach(SERVO3_PIN, 600, 2400);  // 600 и 2400 - длины импульсов
  // servo3.setSpeed(SERVO3_SPEED);
  // servo3.setAccel(SERVO3_ACCEL);
  servo[3].setAutoDetach(0);
  servo[3].write(SERVO3_MIN_ANGLE);

  Serial.begin(9600);
}

bool flag2 = 0;
bool flag3 = 0;
byte cnt2 = 0;
byte cnt3 = 0;
byte s1 = SERVO1_START_ANGLE;
byte s2 = SERVO2_START_ANGLE;
byte s3 = SERVO3_START_ANGLE;

void loop()
{
  btn1.tick();
  btn2.tick();
  btn3.tick();

  servo[1].tick();
  servo[2].tick();
  servo[3].tick();

  Serial.print("Серво 1: ");
  Serial.print(s1);
  Serial.print("  |    Серво 2: ");
  Serial.print(s2);
  Serial.print("  |    Серво 3: ");
  Serial.print(s3);

  // ниже данные для отладки
  Serial.print("  |    cnt2: ");
  Serial.print(cnt2);
  Serial.print("  |    cnt3: ");
  Serial.print(cnt3);
  Serial.print("  |    flag2: ");
  Serial.print(flag2);
  Serial.print("  |    flag3: ");
  Serial.print(flag3);
  Serial.print("  |    ANALOG: ");
  Serial.print(analogRead(ANALOG_INPUT_PIN));
  // Serial.println();

  Watchdog.reset();

  if (analogRead(ANALOG_INPUT_PIN) > ANALOG_VALUE)
  {
    if (btn1.hold()) // кнопка 1
    {
      Serial.println("  |    BTN1 HOLD");

      servo[1].write(SERVO1_MAX_ANGLE);
      s1 = SERVO1_MAX_ANGLE;
      delay(100);
    }
    if (btn2.hold()) // кнопка 2
    {
      Serial.println("  |    BTN2 HOLD");

      if (cnt2 == 0 && flag2 == 0)
      {
        servo[1].write(SERVO1_MAX_ANGLE);
        s1 = SERVO1_MAX_ANGLE;
        cnt2++;
        flag2 = 1;
        delay(100);
      }
      if (cnt2 == 2 && flag2 == 0)
      {
        servo[2].write(SERVO2_MAX_ANGLE);
        s2 = SERVO2_MAX_ANGLE;
        cnt2++;
        flag2 = 1;
        delay(100);
      }
    }
    // if (btn3.hold()) // кнопка 3
    // {
    //   Serial.println("  |    BTN3 HOLD");

    //   if (cnt3 == 0 && flag3 == 0)
    //   {
    //     servo1.write(SERVO1_MAX_ANGLE);
    //     s1 = SERVO1_MAX_ANGLE;
    //     cnt3++;
    //     flag3 = 1;
    //     delay(100);
    //   }
    //   if (cnt3 == 2 && flag3 == 0)
    //   {
    //     servo2.write(SERVO2_MAX_ANGLE);
    //     s2 = SERVO2_MAX_ANGLE;
    //     cnt3++;
    //     flag3 = 1;
    //     delay(100);
    //   }
    //   if (cnt3 == 4 && flag3 == 0)
    //   {
    //     servo3.write(SERVO3_MAX_ANGLE);
    //     s3 = SERVO3_MAX_ANGLE;
    //     cnt3++;
    //     flag3 = 1;
    //     delay(100);
    //   }
    // }
  }
  if (analogRead(ANALOG_INPUT_PIN) <= ANALOG_VALUE)
  {
    if (btn1.hold())
    {
      Serial.println("  |    BTN1 HOLD");

      servo[1].write(SERVO1_MIN_ANGLE);
      s1 = SERVO1_MIN_ANGLE;
      delay(100);
    }
    if (btn2.hold()) // кнопка 2
    {
      Serial.println("  |    BTN2 HOLD");

      if (cnt2 == 1 && flag2 == 1)
      {
        servo[1].write(SERVO1_MIN_ANGLE);
        s1 = SERVO1_MIN_ANGLE;
        cnt2++;
        flag2 = 0;
        delay(100);
      }
      if (cnt2 == 3 && flag2 == 1)
      {
        servo[2].write(SERVO2_MIN_ANGLE);
        s2 = SERVO2_MIN_ANGLE;
        cnt2 = 0;
        flag2 = 0;
        delay(100);
      }
    }
    // if (btn3.hold()) // кнопка 3
    // {
    //   Serial.println("  |    BTN3 HOLD");

    //   if (cnt3 == 1 && flag3 == 1)
    //   {
    //     servo1.write(SERVO1_MIN_ANGLE);
    //     s1 = SERVO1_MIN_ANGLE;
    //     cnt3++;
    //     flag3 = 0;
    //     delay(100);
    //   }
    //   if (cnt3 == 3 && flag3 == 1)
    //   {
    //     servo2.write(SERVO2_MIN_ANGLE);
    //     s2 = SERVO2_MIN_ANGLE;
    //     cnt3++;
    //     flag3 = 0;
    //     delay(100);
    //   }
    //   if (cnt3 == 5 && flag3 == 1)
    //   {
    //     servo3.write(SERVO3_MIN_ANGLE);
    //     s3 = SERVO3_MIN_ANGLE;
    //     cnt3 = 0;
    //     flag3 = 0;
    //     delay(100);
    //   }
    // }
  }
}