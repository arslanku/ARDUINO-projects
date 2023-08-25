#define CLK A5
#define DIO A4

#include <Arduino.h>
#include <Gyver433.h>

#include "GyverTM1637.h"
GyverTM1637 disp(CLK, DIO);

long disp_timer = millis();
#include "keyboard.h"

Gyver433_TX<2> tx; // указали пин

void setup()
{
  Serial.begin(9600);

  disp.clear();
  disp.brightness(7);
  disp.displayByte(_H, _E, _L, _O);
}

void loop()
{
  static int old_data;
  int data = keyboard();
  if (data != old_data)
  {
    old_data = data;

    disp.clear();
    disp.displayInt(data);
    disp_timer = millis();

    tx.sendData(data);
    delay(100);
    tx.sendData(data);
    delay(100);
    tx.sendData(data);
  }

  if (disp_timer - millis() >= 5000)
    disp.clear();
}
