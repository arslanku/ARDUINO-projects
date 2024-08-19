// ==============================================

#define BTN_FORWARD_PIN A2
#define BTN_BACK_PIN A5

#define STEPS_FOR_CIRCLE 200
#define STEPPER_PIN1 9
#define STEPPER_PIN2 10
#define STEPPER_PIN3 11
#define STEPPER_PIN4 12

#define TIMER 10

// ==============================================

#include <Arduino.h>

#include <GyverStepper.h>
GStepper<STEPPER4WIRE> stepper(STEPS_FOR_CIRCLE, STEPPER_PIN1, STEPPER_PIN2, STEPPER_PIN3, STEPPER_PIN4);

#include <GyverButton.h>
GButton btnFwd(BTN_FORWARD_PIN);
GButton btnBack(BTN_BACK_PIN);

uint32_t timer = 0;
bool flag = false;

// ==============================================

void setup()
{ 
  pinMode(A4, OUTPUT);
  digitalWrite(A4, 0);
  pinMode(A0, OUTPUT);
  digitalWrite(A0, 0);

  stepper.setRunMode(KEEP_SPEED);
  stepper.autoPower(true);
  stepper.setAccelerationDeg(360 * 3);
  stepper.stop();

  btnFwd.setTimeout(1000);
  btnBack.setTimeout(1000);

  timer = millis();
}

// ==============================================

void loop()
{
  btnFwd.tick();
  btnBack.tick();
  stepper.tick();

  if ((millis() - timer >= (TIMER * 60000)) && flag)
  {
    stepper.stop();
    flag = false;
  }

  if (btnFwd.isHolded() || btnBack.isHolded())
  {
    stepper.stop();
    flag = false;
  }

  if (btnFwd.isClick())
  {
    stepper.setSpeedDeg(360 * 3);
    timer = millis();
    flag = true;
  }
  else if (btnBack.isClick())
  {
    stepper.setSpeedDeg(-360 * 3);
    timer = millis();
    flag = true;
  }
}
