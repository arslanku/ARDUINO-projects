#define BTN1_PIN 2
#define BTN2_PIN 3
#define BTN3_PIN 4
#define BTN4_PIN 5

#include <Arduino.h>

#include <GyverButton.h>
GButton btn1(BTN1_PIN);

void setup()
{
  btn1.setType(LOW_PULL);
  btn1.setClickTimeout(400);
  btn1.setTimeout(700);
  Serial.begin(9600);
}

void loop()
{
  btn1.tick();
  if (btn1.isSingle())
  {
    Serial.println("1. single");
  }
  // if(btn1.isHold())
  // {
  //   Serial.println("1. hold");
  // }
  // if(btn1.isHolded())
  // {
  //   Serial.println("1. holded");
  // }
  if(btn1.isStep())
  {
    Serial.println("1. step");
  }
  delay(10);
}
