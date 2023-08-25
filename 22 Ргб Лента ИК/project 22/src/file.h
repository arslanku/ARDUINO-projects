#define STRIP_PIN 4   // пин ленты
#define NUMLEDS 180   // кол-во светодиодов
#include <microLED.h> // подключаем библу
microLED<NUMLEDS, STRIP_PIN, -1, LED_WS2812, ORDER_GRB> strip;

void setup()
{
  strip.setBrightness(60); // яркость
}

void loop()
{
  static byte counter = 0;
  for (int i = 0; i < NUMLEDS; i++)
  {
    strip.set(i, mWheel8(counter + i * 255 / NUMLEDS)); // counter смещает цвет
  }
  counter += 3; // counter имеет тип byte и при достижении 255 сбросится в 0
  delay(30);    // 30 кадров в секунду
}



void change_mode()
{
  if (led_mode == 1) // белый
  {
    strip.fill(mWhite);
    strip.show();
  }
  else if (led_mode == 2) // красный
  {
    strip.fill(mRed);
    strip.show();
  }
  else if (led_mode == 3) // зеленый
  {
    strip.fill(mGreen);
    strip.show();
  }
  else if (led_mode == 4) // цвета как на фото
  {
    strip.fill(0, 60, mRGB(56, 148, 223));
    strip.fill(61, 120, mRGB(48, 53, 147));
    strip.fill(121, 180, mRGB(189, 61, 52));
    strip.show();
  }
}