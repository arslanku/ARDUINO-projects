#include <Arduino.h>

// ============== НАСТРОЙКИ ===================

#define Btn_Pin 3    // Пин кнопки D2
#define R_Led 5      // Пин D4, красный светодиод
#define G_Led 7      // Пин D8, зеленый светодиод
#define Y_Led 9      // Пин D6, желтый светодиод
#define buzzerPin 11 // Пин для пищалки

#define buzzerTime 220 // время (в мс) сколько звучит одна нота милодии

int win_melody[] = {100, 300, 500, 800, 600, 800};
int lose_melody[] = {800, 600, 800, 500, 300, 100};

// ============================================

void setup()
{
  pinMode(Btn_Pin, 2);
  pinMode(R_Led, 1);
  pinMode(Y_Led, 1);
  pinMode(G_Led, 1);

  Serial.begin(9600);
}

void loop()
{
  static uint32_t tmr = millis();
  static byte Wmode = 0;

  static uint32_t timer = millis();
  static bool mode = 0;
  static bool winFlag = 0;
  static bool loseFlag = 0;
  static uint32_t tmrLed = millis();

  static bool btnFlag = 0;
  static uint32_t btnTimer = 0;

  if (!mode && (millis() - tmr >= 150))
  {
    tmr = millis();
    switch (Wmode)
    {
    case 0:
      leds(1, 0, 0);
      break;
    case 1:
      leds(0, 1, 0);
      break;
    case 2:
      leds(0, 0, 1);
      break;
    }
    if (Wmode < 2)
      Wmode++;
    else
      Wmode = 0;
  }
  else if (mode)
  {
    if (millis() - timer <= 5000)
    {
      if (millis() - tmrLed >= 750)
      {
        tmrLed = millis();
        leds(0, 0, !digitalRead(G_Led));
      }
    }
    else if (millis() - timer <= 10000)
    {
      if (millis() - tmrLed >= 750)
      {
        tmrLed = millis();
        leds(0, !digitalRead(Y_Led), 0);
      }
    }
    else if (millis() - timer < 15000)
    {
      if (millis() - tmrLed >= 750)
      {
        tmrLed = millis();
        leds(!digitalRead(R_Led), 0, 0);
      }
    }
    if (millis() - timer >= 15000)
    {
      loseFlag = 1;
      mode = 0;
      leds(0, 0, 0);
    }
  }

  bool btnState = !digitalRead(Btn_Pin);
  if (btnState && !btnFlag && millis() - btnTimer > 200)
  {
    btnFlag = true;
    btnTimer = millis();
    Serial.println("press");
    if (!mode)
    {
      mode = 1;
      timer = millis();
      leds(0, 0, 0);
    }
    else
    {
      winFlag = 1;
      leds(0, 0, 0);
      mode = 0;
    }
  }
  if (!btnState && btnFlag && millis() - btnTimer > 200)
  {
    btnFlag = false;
    btnTimer = millis();
    Serial.println("release");
  }

  if (winFlag)
  {
    for (byte i = 0; i < sizeof(win_melody) / sizeof(win_melody[0]); i++)
    {
      digitalWrite(G_Led, !digitalRead(G_Led));
      tone(buzzerPin, win_melody[i], buzzerTime);
      delay(buzzerTime + 50);
    }
    noTone(buzzerPin);
    digitalWrite(G_Led, 1);
    winFlag = 0;
  }

  if (loseFlag)
  {
    for (byte i = 0; i < sizeof(lose_melody) / sizeof(lose_melody[0]); i++)
    {
      digitalWrite(R_Led, !digitalRead(R_Led));
      tone(buzzerPin, lose_melody[i], buzzerTime);
      delay(buzzerTime + 50);
    }
    noTone(buzzerPin);
    digitalWrite(R_Led, 0);
    loseFlag = 0;
  }
}

void leds(bool R, bool Y, bool G)
{
  digitalWrite(R_Led, R);
  digitalWrite(Y_Led, Y);
  digitalWrite(G_Led, G);
}
