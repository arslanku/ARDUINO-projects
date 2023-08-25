// ===== настройки ==========================

#define START_BRIGHTNESS 200 // начальная яркость (от 0 - до 250)
#define NUMLEDS 180          // общее кол-во светодиодов [60 * 7]
#define COLOR_DEBTH 3        // глубина (качество) цвета. (от 1 - до 3). где 1-минимум, 3-максимум

#define STRIP_PIN 12 // пин для подключения логического сигнала к ленте
#define BTN_PIN 3    // пин для подключения кнопки
#define RC_PIN 2     // пин для подключения пульта

// ==========================================

#define IR_1 0xA2
#define IR_2 0x62
#define IR_3 0xE2
#define IR_4 0x22
#define IR_5 0x2
#define IR_UP 0x18
#define IR_DOWN 0x4A

#include <Arduino.h>

#include <microLED.h>
microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2818, ORDER_GRB, CLI_AVER> strip;

#include <GyverButton.h>
GButton btn(BTN_PIN);

#include <NecDecoder.h>
NecDecoder rc;

bool flag2 = 1;
byte led_mode = 1;
byte brightness = START_BRIGHTNESS;

void RC_ISR()
{
  rc.tick();
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Hello");

  strip.setBrightness(START_BRIGHTNESS);

  btn.setStepTimeout(100);

  attachInterrupt(0, RC_ISR, FALLING);
}

void rainbow()
{
  static byte counter = 0;
  for (int i = 0; i < NUMLEDS; i++)
    strip.set(i, mWheel8(counter + i));

  counter += 1;
  strip.show();
}

void button()
{
  btn.tick();

  if (btn.isClick())
  {
    if (led_mode == 5)
    {
      led_mode = 1;
    }
    else
    {
      led_mode++;
    }
  }

  if (btn.isStep())
  {
    if (flag2 && brightness < 250)
      brightness += 10;

    if (!flag2 && brightness > 30)
      brightness -= 10;

    Serial.println(brightness);
  }

  if (btn.isHolded())
    flag2 = !flag2;

  if (led_mode == 1) // белый
  {
    strip.setBrightness(brightness);
    strip.fill(mWhite);
    strip.show();
  }
  else if (led_mode == 2) // красный
  {
    strip.setBrightness(brightness);
    strip.fill(mRed);
    strip.show();
  }
  else if (led_mode == 3) // зеленый
  {
    strip.setBrightness(brightness);
    strip.fill(mGreen);
    strip.show();
  }
  else if (led_mode == 4) // цвета как на фото
  {
    strip.fill(0, 60, mRGB(56, 148, 223));
    strip.fill(61, 120, mRGB(48, 53, 147));
    strip.fill(121, 180, mRGB(189, 61, 52));

    strip.setBrightness(brightness);
    strip.show();
  }
  else if (led_mode == 5) // радуга
  {
    strip.setBrightness(brightness);
    rainbow();
  }
}

void remote_control()
{
  if (rc.available())
  {
    Serial.println(rc.readCommand(), HEX);

    switch (rc.readCommand())
    {
    case IR_1:
      led_mode = 1;
      break;
    case IR_2:
      led_mode = 2;
      break;
    case IR_3:
      led_mode = 3;
      break;
    case IR_4:
      led_mode = 4;
      break;
    case IR_5:
      led_mode = 5;
      break;

    case IR_UP:
      if (brightness < 250)
        brightness += 10;
      break;
    case IR_DOWN:
      if (brightness > 30)
        brightness -= 10;
      break;
    }
  }
}

void loop()
{
  button();
  remote_control();

  Serial.print(led_mode);
  Serial.print("; ");
  Serial.println(brightness);
}