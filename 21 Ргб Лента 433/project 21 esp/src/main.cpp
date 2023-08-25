// ===== настройки =====

#define NUMLEDS 420 // общее кол-во светодиодов (60 * 7)
#define STRIP_PIN 2 // пин для подключения логического сигнала к ленте

#define WIFI_SSID "LED control"
#define WIFI_PASSWORD "12345687"

// =====================

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <GyverPortal.h>
GyverPortal portal;


void build()
{
}

void action(GyverPortal &p)
{
}

void CONNECT_PORTAL()
{
  portal.attachBuild(build);
  portal.attach(action);
  portal.start();

  Serial.println("Портал запущен!");
}

void CONNECT_WIFI()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("");
  Serial.println("WIFI запущен!");
}

void setup()
{
  Serial.begin(115200);

  CONNECT_PORTAL();
  CONNECT_WIFI();

  pinMode(2, 1);
}

void loop()
{
  static uint32_t timer;
  if (millis() - timer >= 500)
  {
    timer = millis();
    digitalWrite(2, !digitalRead(2));
  }
}
