#include <Arduino.h>

const int buttonPins[6] = {A0, A1, A2, A3, A4, A5};
const int ledPins[12] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
bool btnState[6];

#include <EncButton.h>
Button btns[6];

void setup()
{
  randomSeed((micros() + analogRead(A7)) / 100);

  Serial.begin(9600);
  Serial.println("start");

  for (int i = 0; i < 6; i++)
  {
    btns[i].init(buttonPins[i]);
  }
  for (int i = 0; i < 12; i++)
  {
    pinMode(ledPins[i], OUTPUT);
  }
}

int randomLed;
bool clicked = false;

void loop()
{
  for (int i = 0; i < 6; i++)
  {
    btns[i].tick();
    if (btns[i].click())
    {
      if (!clicked && !btnState[i])
      {
        btnState[i] = true;
        clicked = true;

        randomLed = random(12);
        digitalWrite(ledPins[randomLed], HIGH);
      }
      else if (clicked && btnState[i])
      {
        btnState[i] = false;
        clicked = false;
        digitalWrite(ledPins[randomLed], LOW);
      }

      Serial.print(i);
      Serial.print(ledPins[randomLed]);
      Serial.println(digitalRead(ledPins[randomLed]));
    }
  }
}