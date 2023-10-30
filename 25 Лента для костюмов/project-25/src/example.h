#define BTN1_PIN 6
#define BTN2_PIN 5
#define BTN3_PIN 4
#define BTN4_PIN 3
// 7 ПИН НЕ ТРОГАТЬ

#define START_BRIGHTNESS 3 // начальная яркость (от 1 - до 5)
#define COLOR_DEBTH 3      // глубина (качество) цвета. (от 1 - до 3). 1-минимум, 3-максимум
#define NUMLEDS 301        // кол-во светодиодов
//              420

#define STRIP_PIN A4 // пин для подключения логического сигнала к ленте

#include <Arduino.h>

#include <GyverButton.h>
GButton btn_a(BTN1_PIN, LOW_PULL);
GButton btn_b(BTN2_PIN, LOW_PULL);
GButton btn_c(BTN3_PIN, LOW_PULL);
GButton btn_d(BTN4_PIN, LOW_PULL);

#include <microLED.h>
microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER> strip;
//                                          LED_WS2815 !

#include <GyverWDT.h>

byte brightness_mode = START_BRIGHTNESS; // (1, 2, 3, 4, 5)
byte brightness = brightness_mode * 50;
byte speed = 1;
byte fill_speed = 10;
byte led_mode = 1;
bool flag = 0;
bool flag2 = 0;

#include "effects.h"

void setup()
{
  Watchdog.enable(RST_MODE, WDT_TIMEOUT_8S);

  Serial.begin(9600);
  Serial.println("Start");

  pinMode(2, 1);
  pinMode(A2, 1);
  pinMode(A3, 1);
  pinMode(A5, 1);
  pinMode(A6, 1);

  digitalWrite(2, 1);
  digitalWrite(A2, 0);
  digitalWrite(A3, 0);
  digitalWrite(A5, 0);
  digitalWrite(A6, 0);

  pinMode(13, 1);

  btn_a.setTimeout(600);

  btn_a.setClickTimeout(500);
  btn_b.setClickTimeout(500);
  btn_c.setClickTimeout(500);
  btn_d.setClickTimeout(500);

  strip.clear();
}

void btn_a_b()
{
  if (btn_a.isHold())
  {
    led_mode = 0;
    Serial.println(led_mode);
  }

  if (btn_a.isSingle() && led_mode > 1)
  {
    led_mode--;
    flag2 = 0;
    Serial.println(led_mode);
  }

  if (btn_b.isSingle() && led_mode < 9)
  {
    led_mode++;
    flag2 = 0;
    Serial.println(led_mode);
  }
}

void btn_c_d()
{
  if (btn_c.isSingle())
  {
    if (brightness_mode >= 5)
      brightness_mode = 1;
    else
      brightness_mode++;

    Serial.println(brightness_mode);
  }

  if (btn_d.isSingle())
  {
    if (speed >= 5)
      speed = 1;
    else
      speed++;

    Serial.println(speed);
  }
}

void loop()
{
  Watchdog.reset();

  btn_a.tick();
  btn_b.tick();
  btn_c.tick();
  btn_d.tick();

  brightness = brightness_mode * 50;
  strip.setBrightness(brightness);
  strip.show();

  btn_a_b();
  btn_c_d();

  if (led_mode == 0)
  {
    strip.clear();
  }

  else if (led_mode == 1 && !flag2) // белый сегмент с пробелами
  {
    strip.clear();
    for (int i = 0; i <= (NUMLEDS - 30); i += 30)
    {
      strip.fill(i, i + 30, mWhite);
      // strip.fill(i + 31, i + 40, mBlack);
      i += 10;
      strip.show();
      delay(100);
    }

    Serial.println("flag2");
    flag2 = 1;
  }

  else if (led_mode == 2 && !flag2) // белый сегмент без пробелов
  {
    strip.clear();
    for (int i = 0; i <= (NUMLEDS - 30); i += 30)
    {
      strip.fill(i, i + 30, mWhite);
      strip.show();
      delay(100);
    }

    Serial.println("flag2");
    flag2 = 1;
  }

  else if (led_mode == 3 && !flag2) // белый
  {
    for (int i = 0; i < NUMLEDS; i += fill_speed)
    {
      strip.fill(0, i, mWhite);
      strip.show();
    }
    flag2 = 1;
  }

  else if (led_mode == 4 && !flag2) // красный
  {
    for (int i = 0; i < NUMLEDS; i += fill_speed)
    {
      strip.fill(0, i, mRed);
      strip.show();
    }
    flag2 = 1;
  }

  else if (led_mode == 5 && !flag2) // зеленый
  {
    for (int i = 0; i < NUMLEDS; i += fill_speed)
    {
      strip.fill(0, i, mGreen);
      strip.show();
    }
    flag2 = 1;
  }

  else if (led_mode == 6 && !flag2) // синий
  {
    for (int i = 0; i < NUMLEDS; i += fill_speed)
    {
      strip.fill(0, i, mBlue);
      strip.show();
    }
    flag2 = 1;
  }

  else if (led_mode == 8)
  {
    rainbow();
  }

  else if (led_mode == 9)
  {
    colorCycle();
  }

  else if (led_mode == 10)
  {
    runningDots();
  }

  static uint32_t timer;
  if (millis() - timer >= 500)
  {
    timer = millis();
    digitalWrite(13, !digitalRead(13));
  }
}
