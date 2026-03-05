#include <Arduino.h>

// яркость днем (0 - 255)
#define DAY_BRIGHTNESS 100

// яркость ночного режима (0 - 255)
#define NIGHT_BRIGHTNESS 30

// теплота света
#define KELVIN 3500

// цифровой пин подключения датчика1
#define SENSOR1_PIN 4

// цифровой пин подключения датчика2
#define SENSOR2_PIN 6

// аналоговый пин подключения фоторезистора
#define PHOTORESISTOR_PIN A0

// значение фоторезистора, ниже которого ночь (0 - 1023)
#define NIGHT_VALUE 400

// задержка между включением ступеней (в мс)
#define DELAY_AFTER_STAGE 100

// время работы подсветки (в мс)
#define WORKING_TIME 5000

// пин подключения ленты
#define STRIP_PIN 3

#define NUMLEDS 492
#define COLOR_DEBTH 3
int led_on_stages[] = {26, 40, 40, 40, 40, 26, 40, 40, 40, 40, 40, 40, 40};

#include <microLED.h>

// microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2811, ORDER_GBR, CLI_AVER> strip;
microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER> strip;
// microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2813, ORDER_GRB, CLI_AVER> strip;
// microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2815, ORDER_GRB, CLI_AVER> strip;
// microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2818, ORDER_RGB, CLI_AVER> strip;
// microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS6812, ORDER_RGB, CLI_AVER> strip;

void setup()
{
  digitalWrite(2, 1);
  pinMode(2, 1);
  digitalWrite(4, 0);
  pinMode(4, 1);

  Serial.begin(9600);

  strip.setBrightness(DAY_BRIGHTNESS);
  strip.clear();
  strip.show();
  delay(1);
}

uint32_t timer = 0;

bool signal1 = false;
bool signal2 = false;
int brightness_value = 0;

void loop()
{
  // bool signal1 = digitalRead(SENSOR1_PIN);
  // bool signal2 = digitalRead(SENSOR2_PIN);
  // int brightness_value = analogRead(PHOTORESISTOR_PIN);

  if (Serial.available())
  {
    int data = Serial.parseInt();
    if (data == 1)
      signal1 = false;
    else if (data == 2)
      signal1 = true;
    else if (data == 3)
      signal2 = false;
    else if (data == 4)
      signal2 = true;
    else
      brightness_value = data;

    Serial.println("");
    Serial.println(data);
    Serial.println(signal1);
    Serial.println(signal2);
    Serial.println(brightness_value);
  }

  if (signal1 || signal2)
  {
    timer = millis();
  }

  if (millis() - timer < WORKING_TIME)
  {
    strip.setBrightness(DAY_BRIGHTNESS);
    int led_cnt = 0;
    for (int i = 0; i < 13; i++)
    {
      led_cnt += led_on_stages[i];
      strip.fill(0, led_cnt - 1, mKelvin(KELVIN));
      strip.show();
      delay(DELAY_AFTER_STAGE);
    }
  }
  else
  {
    if (brightness_value < NIGHT_VALUE)
    {
      strip.setBrightness(NIGHT_BRIGHTNESS);
      strip.clear();
      strip.fill(0, 26 - 1, mKelvin(KELVIN));
      strip.fill(452, 492 - 1, mKelvin(KELVIN));
      strip.show();
      delay(1);
    }
    else
    {
      strip.clear();
      strip.show();
      delay(1);
    }
  }
}