// ======================= НАСТРОЙКИ =======================

#define SENSOR_PIN 2 // пин подключения датчика

#define SENSOR_VALUE_MIN 0    // минимальное показание датчика
#define SENSOR_VALUE_MAX 1023 // макимальное показание датчика

#define STEP_PIN 8 // пин для подключения STEP пина
#define DIR_PIN 6  // пин для подключения DIR пина

#define MOTOR_SPEED 150  // скорость мотора (шагов/сек)
#define MOTOR_ACCEL 1    // величина ускорения/торможения (шаг/сек^2)
#define AUTO_POWER false // отключать мотор при достижении цели (true/false)

#define STEPS_PER_DEGREE 1.8 // сколько градусов на 1 шаг
#define STEPS_PER_CIRCLE 200 // сколько шагов на 360 градусов
/*
  в интернете нашел информацию что за один шаг мотор 42HS40-1304A05-D24 поварачивается на 1.8 градусов,
  поэтому STEPS_PER_DEGREE будет 1.8, а соотвественно STEPS_PER_CIRCLE будет 200 (т.к. 360 / 1.8 = 200)
*/

#define ANGLE_MIN 0    // минимальный угол
#define ANGLE_MAX 2160 // максимальный угол (6 оборотов по 360 градусов = 2160)

#define MODE 1 // режимы работы (1, 2, 3)
/*
  1 - режим работы по оборотам в минуту
  2 - режим работы по частоте (Гц)
  3 - режим работы по периоду
*/

// ==========================================================



#include "StepperLib.h"
GStepper2<STEPPER2WIRE> stepper(STEPS_PER_CIRCLE, STEP_PIN, DIR_PIN);

#include "Tacho.h"
Tachometer tacho;

void motor_setup()
{
  stepper.setTarget(0);

  stepper.autoPower(AUTO_POWER);

  stepper.setMaxSpeed(MOTOR_SPEED);

  stepper.setAcceleration(MOTOR_ACCEL);
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Начало работы");

  motor_setup();
  Serial.println("Мотор подключен");

  attachInterrupt(0, sensor, RISING);
  Serial.println("Датчик подключен");
}

void loop()
{
  stepper.tick();

  static int value = 0;
  static int steps = 0;
  static byte angle = 0;

  Serial.print("Значение с датчика: ");
  Serial.println(value);

  if (MODE == 1)
    value = tacho.getRPM();
  else if (MODE == 2)
    value = tacho.getHz();
  else if (MODE == 3)
    value = tacho.getUs();

  angle = map(value, SENSOR_VALUE_MIN, SENSOR_VALUE_MAX, ANGLE_MIN, ANGLE_MAX);

  steps = angle / STEPS_PER_DEGREE;
  stepper.setTarget(steps);

  Serial.print("Значение угла: ");
  Serial.println(angle);
}

void sensor()
{
  tacho.tick();
}