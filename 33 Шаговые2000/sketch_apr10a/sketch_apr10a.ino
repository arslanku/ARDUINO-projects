#include <Arduino.h>

#include <GyverStepper.h>
GStepper<STEPPER4WIRE> stepper(200, 9, 10, 11, 12);

void setup()
{
  Serial.begin(9600);
  stepper.setRunMode(KEEP_SPEED);
  stepper.setAccelerationDeg(360 * 2);
  stepper.setSpeedDeg(-360 * 3);
  stepper.autoPower(true);
}

void loop()
{
  stepper.tick();
}
