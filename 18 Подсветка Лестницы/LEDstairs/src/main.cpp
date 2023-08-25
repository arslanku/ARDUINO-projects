// ============== НАСТРОЙКИ =====================

#define BRIGHTNESS 200 // максимальная яркость ленты

#define LED_PIN 12    // пин для подключения ленты
#define NUM_LEDS 70    // кол-во светодиодов
#define COLOR_DEBTH 1 // глубина цвета

#define SENSOR_PIN1 2
#define SENSOR_PIN2 3

#define DELAY_TIME 15000 // время после которого лента отключится (в миллисекундах!)

// ==============================================

#include <GyverWDT.h>

#include <microLED.h>
// microLED<NUM_LEDS, LED_PIN, MLED_NO_CLOCK, LED_WS2815, ORDER_GRB, CLI_AVER, SAVE_MILLIS> strip;
microLED<NUM_LEDS, LED_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER, SAVE_MILLIS> strip;

#include "GyverTimer.h"
GTimer timer(MS);

void setup()
{
  Watchdog.enable(RST_MODE, WDT_TIMEOUT_2S);

  strip.setBrightness(0);
  strip.clear();
  strip.show();

  timer.setTimeout(DELAY_TIME);
  timer.stop();
  timer.reset();

  Serial.begin(9600);
}

bool on = 0;
int value = 0;
void loop()
{
  if ((digitalRead(SENSOR_PIN1) || digitalRead(SENSOR_PIN2)))
  {
    Serial.println("Move detected!");
    timer.start();

    if (!on)
    {
      Serial.println("On");
      strip.fill(mWhite);
      for (byte i = 20; i <= BRIGHTNESS; i++)
      {
        strip.setBrightness(i);
        strip.show();
        delay(5);
      }
      on = 1;
    }
  }

  if (timer.isReady() && on)
  {
    timer.stop();
    Serial.println("Off");
    for (byte i = BRIGHTNESS; i > 0; i--)
    {
      Serial.println(i);
      strip.setBrightness(i);
      strip.show();
      delay(5);
    }
    on = 0;
  }

  Watchdog.reset();
}