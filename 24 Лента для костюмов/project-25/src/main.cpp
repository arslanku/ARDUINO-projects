#define BTN1_PIN 6
#define BTN2_PIN 5
#define BTN3_PIN 4
#define BTN4_PIN 3

#define COLOR_DEBTH 3 // глубина (качество) цвета. (от 1 - до 3). 1-минимум, 3-максимум
#define NUMLEDS 101   // кол-во светодиодов
#define STRIP_PIN A0
#define BRIGHTNESS 100

#define EFFECT1 10000
#define EFFECT2 25000
#define EFFECT3 27000
#define EFFECT4 30000

#include <Arduino.h>

#include <GyverButton.h>
GButton btn_a(BTN1_PIN, LOW_PULL);
GButton btn_b(BTN2_PIN, LOW_PULL);
GButton btn_c(BTN3_PIN, LOW_PULL);
GButton btn_d(BTN4_PIN, LOW_PULL);

#include <microLED.h>
microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER> strip;

uint32_t timer;
uint32_t led_timer;
bool start;

void setup()
{
  pinMode(13, 1);

  btn_a.setTimeout(800);

  btn_a.setClickTimeout(700);
  btn_b.setClickTimeout(700);
  btn_c.setClickTimeout(700);
  btn_d.setClickTimeout(700);

  strip.clear();

  start = 1;
  timer = millis();
}

void rainbow()
{
  static byte counter = 0;
  for (int i = 0; i < NUMLEDS; i++)
  {
    strip.set(i, mWheel8(counter + i * 255 / NUMLEDS)); // counter смещает цвет
  }
  strip.show();
  counter += 3; // counter имеет тип byte и при достижении 255 сбросится в 0
}

void loop()
{
  strip.setBrightness(BRIGHTNESS);

  // if (btn_a.isClick())
  // {
  //   start = 1;
  //   timer = millis();
  //   strip.setBrightness(BRIGHTNESS);
  // }

  timer = millis();

  if (start)
  {
    if (timer < EFFECT1) // стробоскоп
    {
      for (byte i = 1; i < 20; i++)
      {
        if (i % 2 == 0)
        {
          strip.fill(0, NUMLEDS - 1, mWhite);
          strip.show();
          delay(EFFECT1 / 100);
        }
        else
        {
          strip.clear();
          strip.show();
          delay(EFFECT1 / 100);
        }
      }
    }
    else if (timer >= EFFECT1 and timer < EFFECT2) // переливание цветов
    {
      rainbow();
    }
    else if (timer >= EFFECT2 and timer <= EFFECT3) // из красного в белый
    {
      for (byte i = 0; i < 255; i++)
      {
        strip.fill(0, NUMLEDS - 1, mRGB(255, i, i));
        strip.show();
        delay((EFFECT3 - EFFECT2) / 255);
      }
    }
    else if (timer >= EFFECT3 and timer < EFFECT4) // плавное затухание
    {
      for (byte i = BRIGHTNESS; i > 0; i--)
      {
        strip.setBrightness(i);
        strip.show();
        delay((EFFECT4 - EFFECT3) / BRIGHTNESS);
      }
    }
    else if (timer >= EFFECT4) // выключение
    {
      start = 0;
      strip.clear();
      strip.show();
    }
  }

  if (millis() - led_timer >= 500)
  {
    led_timer = millis();
    digitalWrite(13, !digitalRead(13));
  }
}