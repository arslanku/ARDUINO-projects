#include <Arduino.h>

// пин потенциометра
#define POTENTIOMETR_PIN A0

// установка ПОСТОЯННОЙ скорости в шагах/сек
#define SPEED_MOTOR1 200

// пин подключения
#define STEP_MOTOR1_PIN 2

// пин подключения
#define DIR_MOTOR2_PIN 3

// установка МАКСИМАЛЬНОЙ скорости (в шагах/сек)
#define MAX_SPEED_MOTOR2 200

// установка ускорения (в шагах/сек/сек)
#define ACCELERATION_MOTOR2 200

// установка максимальной позиции мотора (в шагах)
#define MAX_MOTOR2_POSITION 2000

// пин подключения
#define STEP_MOTOR2_PIN 5

// пин подключения
#define DIR_MOTOR2_PIN 6

// количество микрошагов ("множитель"). нужно будет отрегулировать
#define STEPS_K 1

#include <GyverStepper.h>

GStepper<STEPPER2WIRE> stepper1(200 * STEPS_K, STEP_MOTOR1_PIN, DIR_MOTOR2_PIN); // драйвер step-dir
GStepper<STEPPER2WIRE> stepper2(200 * STEPS_K, STEP_MOTOR2_PIN, DIR_MOTOR2_PIN); // драйвер step-dir

void setup()
{
  stepper1.setRunMode(KEEP_SPEED);
  stepper1.setSpeed(SPEED_MOTOR1);

  stepper2.setRunMode(FOLLOW_POS);
  stepper2.setMaxSpeed(MAX_SPEED_MOTOR2);
  stepper2.setAcceleration(ACCELERATION_MOTOR2);
}

void loop()
{
  stepper1.tick();

  if (!stepper2.tick())
  {
    static bool dir;
    dir = !dir;
    stepper2.setTarget(dir ? 0 : MAX_MOTOR2_POSITION);
  }

  static uint32_t timer;
  if (millis() - timer > 100)
  {
    timer = millis();
    int speed = analogRead(POTENTIOMETR_PIN);
    stepper1.setSpeed(speed);
    stepper2.setMaxSpeed(speed);
    stepper2.setAcceleration(speed);
  }
}
