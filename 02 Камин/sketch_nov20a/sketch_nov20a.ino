// ====================== НАСТРОЙКИ ======================



#define SERVO_SPEED 90   // скорость сервоприводов (градусов/сек)
#define SERVO_ACCEL 0.8  // ускорение сервоприводов (для плавного пуска и торможения) от 0.05 до 1
#define START_ANGLE 0    // начальный угол сервпориводов (0 градусов)

#define TEMP_MIN 30      // температура, при угле в 0 градусов на сервоприводе
#define TEMP_MAX 50      // температура, при угле в 90 градусов на сервоприводе

#define PIN_SERVO1 9      // пин сервопривода №1
#define PIN_SERVO2 10     // пин сервопривода №2
#define PIN_SERVO3 11     // пин сервопривода №3

#define PIN_BTN 6         // пин кнопки

#define PIN_THERM1 A0     // пин датчика температуры №1
#define PIN_THERM2 A2     // пин датчика температуры №2
#define PIN_THERM3 A4     // пин датчика температуры №3

#define PIN_DISP_CLK 3
#define PIN_DISP_DIO 4




// =======================================================

// =========== НИЖЕ НИЧЕГО НЕ МЕНЯТЬ! ====================

// =========== НИЖЕ НИЧЕГО НЕ МЕНЯТЬ! ====================

// =========== НИЖЕ НИЧЕГО НЕ МЕНЯТЬ! ====================






















// ====================== НИЖЕ НИЧЕГО НЕ МЕНЯТЬ! ==============================


#include "ServoSmooth.h"
#include "GyverNTC.h"
#include "GyverTM1637.h"
#include "GyverTimer.h"
#include "EncButton.h"

ServoSmooth servo1;
ServoSmooth servo2;
ServoSmooth servo3;

GyverNTC therm1(PIN_THERM1, 10000, 3950);
GyverNTC therm2(PIN_THERM2, 10000, 3950);
GyverNTC therm3(PIN_THERM3, 10000, 3950);

GyverTM1637 disp(PIN_DISP_CLK, PIN_DISP_DIO);

GTimer timer1(MS);

EncButton<EB_TICK, PIN_BTN> btn1;


void setup() {
  servo1.attach(PIN_SERVO1);         // устанавливаем пин для поключения (9)
  servo1.setSpeed(SERVO_SPEED);      // устанавливаем скорость (градусов/сек)
  servo1.setAccel(SERVO_ACCEL);      // устанавливаем ускорение (для плавного пуска и торможения)
  servo1.setTargetDeg(START_ANGLE);  // устанавливаем начальный угол (0 градусов)

  servo2.attach(PIN_SERVO2);         // устанавливаем пин для поключения (10)
  servo2.setSpeed(SERVO_SPEED);      // устанавливаем скорость (градусов/сек)
  servo2.setAccel(SERVO_ACCEL);      // устанавливаем ускорение (для плавного пуска и торможения)
  servo1.setTargetDeg(START_ANGLE);  // устанавливаем начальный угол (0 градусов)

  servo3.attach(PIN_SERVO3);         // устанавливаем пин для поключения (11)
  servo3.setSpeed(SERVO_SPEED);      // устанавливаем скорость (градусов/сек)
  servo3.setAccel(SERVO_ACCEL);      // устанавливаем ускорение (для плавного пуска и торможения)
  servo3.setTargetDeg(START_ANGLE);  // устанавливаем начальный угол (0 градусов)

  disp.clear();
  disp.brightness(7);  // яркость, 0 - 7 (минимум - максимум)
  disp.clear();

  timer1.setInterval(1000);

  Serial.begin(9600);
}

void loop() {
  static byte disp_temp = 1;

  servo1.tick();
  servo2.tick();
  servo3.tick();

  btn1.tick();

  static float tmp1, tmp2, tmp3;
  static uint8_t angle1, angle2, angle3;

  tmp1 = therm1.getTempAverage();
  tmp2 = therm2.getTempAverage();
  tmp3 = therm3.getTempAverage();

  byte byte_tmp1 = tmp1, byte_tmp2 = tmp2, byte_tmp3 = tmp3;

  angle1 = map(tmp1, TEMP_MIN, TEMP_MAX, 0, 90);
  angle2 = map(tmp2, TEMP_MIN, TEMP_MAX, 0, 90);
  angle3 = map(tmp3, TEMP_MIN, TEMP_MAX, 0, 90);

  if (btn1.hold()) {
    servo1.setTargetDeg(angle1);
    servo2.setTargetDeg(angle2);
    servo3.setTargetDeg(angle3);
  }


  if (timer1.isReady()) {
    switch (disp_temp) {
      case 1:
        disp.clear();
        disp.displayInt(byte_tmp1);
        disp_temp = 2;
        break;
      case 2:
        disp.clear();
        disp.displayInt(byte_tmp2);
        disp_temp = 3;
        break;
      case 3:
        disp.clear();
        disp.displayInt(byte_tmp3);
        disp_temp = 1;
        break;
    }
  }
}