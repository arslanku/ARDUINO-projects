// ===================================================

// НАЗВАНИЕ СЕТИ ПО УМОЛЧАНИЮ: "ESP Portal"
// ПАРОЛЬ ОТ СЕТИ: 123456789

// ===================================================

#define WIFI_SSID "ESP Portal"
#define WIFI_PASSWORD "12345687"

#define LED_PIN 2

#define SENSOR_PIN1 13
#define SENSOR_PIN2 12
#define SENSOR_PIN3 14
#define SENSOR_PIN4 16

#define RELAY_PIN1 3
#define RELAY_PIN2 5
#define RELAY_PIN3 4
#define RELAY_PIN4 0
#define RELAY_PIN5 2
#define RELAY_PIN6 15

#define INTERVAL 500

// ===================================================

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <GyverPortal.h>

#include <EEManager.h>

#include <microDS18B20.h>
#include "GyverTimer.h"

byte temp[4];
byte temp_max[5];
bool led_state[4];
bool relay_state[6];

GyverPortal portal;

EEManager mem1(temp_max);

MicroDS18B20<SENSOR_PIN1> sensor1;
MicroDS18B20<SENSOR_PIN2> sensor2;
MicroDS18B20<SENSOR_PIN3> sensor3;
MicroDS18B20<SENSOR_PIN4> sensor4;

GTimer sensor_timer(MS);

#include "build.h"
#include "action.h"

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
  EEPROM.begin(mem1.blockSize());

  mem1.begin(0, 'a');

  Serial.println(temp_max[0]);
  Serial.println(temp_max[1]);
  Serial.println(temp_max[2]);
  Serial.println(temp_max[3]);
  Serial.println(temp_max[4]);
  Serial.println(temp_max[5]);

  CONNECT_PORTAL();
  CONNECT_WIFI();

  sensor_timer.setInterval(INTERVAL);

  // Serial.begin(115200);

  // pinMode(LED_PIN, OUTPUT);
  // digitalWrite(LED_PIN, 0);

  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);
  pinMode(RELAY_PIN3, OUTPUT);
  pinMode(RELAY_PIN4, OUTPUT);
  pinMode(RELAY_PIN5, OUTPUT);
  pinMode(RELAY_PIN6, OUTPUT);
}

void loop()
{
  portal.tick();
  mem1.tick();
  // delayMicroseconds(5);

  if (sensor_timer.isReady())
  {
    if (sensor1.readTemp())
      temp[0] = sensor1.getTemp();
    if (sensor2.readTemp())
      temp[1] = sensor2.getTemp();
    if (sensor3.readTemp())
      temp[2] = sensor3.getTemp();
    if (sensor4.readTemp())
      temp[3] = sensor4.getTemp();
    else
      Serial.println("error");

    sensor1.requestTemp();
    sensor2.requestTemp();
    sensor3.requestTemp();
    sensor4.requestTemp();

    temp[0] = random(100);
    temp[1] = random(100);
    temp[2] = random(100);
    temp[3] = random(100);
    temp[4] = random(100);
    temp[5] = random(100);

    // digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }

  if (temp[0] < temp_max[0]) // sensor 1
    relay_state[0] = 0;
  else
    relay_state[0] = 1;

  if (temp[0] < temp_max[1]) // sensor 1
    relay_state[1] = 0;
  else
    relay_state[1] = 1;

  if (temp[0] < temp_max[2]) // sensor 1
    relay_state[2] = 0;
  else
    relay_state[2] = 1;

  if (temp[1] < temp_max[3]) // sensor 2
    relay_state[3] = 0;
  else
    relay_state[3] = 1;

  if (temp[1] < temp_max[4]) // sensor 2
    relay_state[4] = 0;
  else
    relay_state[4] = 1;

  if (temp[2] < temp_max[5]) // sensor 3
    led_state[2] = 0;
  else
    led_state[2] = 1;

  digitalWrite(RELAY_PIN1, relay_state[0]);
  digitalWrite(RELAY_PIN2, relay_state[1]);
  digitalWrite(RELAY_PIN3, relay_state[2]);
  digitalWrite(RELAY_PIN4, relay_state[3]);
  digitalWrite(RELAY_PIN5, relay_state[4]);
  digitalWrite(RELAY_PIN6, relay_state[5]);
}
