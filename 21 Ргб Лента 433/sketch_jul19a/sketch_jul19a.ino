// ===== настройки =====

#define NUMLEDS 420 // общее кол-во светодиодов (60 * 7)
#define STRIP_PIN 2 // пин для подключения логического сигнала к ленте

// =====================

#include <Arduino.h>

// #include <microLED.h>
// microLED<NUMLEDS, STRIP_PIN, -1, LED_WS2815, ORDER_GRB> strip;

#include <GyverHub.h>
GyverHub hub("MyDevices", "ESP8266"); // префикс, имя, иконка

void wifi_on()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin("TP-Link", "indira777semya");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED)
    Serial.println("Connected!");
}

void build() {}

void setup()
{
  Serial.begin(115200);

  wifi_on(); 

  hub.onBuild(build);
  hub.begin();

  pinMode(2, 1);
}

void loop()
{
  hub.tick();

  // static uint32_t timer;
  // if (millis() - timer >= 500)
  // {
  //   timer = millis();
  //   digitalWrite(2, !digitalRead(2));
  // }
}
