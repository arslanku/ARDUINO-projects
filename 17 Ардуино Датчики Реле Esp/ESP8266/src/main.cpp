// ===================================================

// НАЗВАНИЕ СЕТИ ПО УМОЛЧАНИЮ: "ESP Portal"
// ПАРОЛЬ ОТ СЕТИ: 123456789

// ===================================================

#define WIFI_SSID "ESP Portal"
#define WIFI_PASSWORD "12345687"

#define LED_PIN 2

#define UART_PIN 0

#define INTERVAL 500

#define MY_ADDRESS 5
#define BUFFER 64

// ===================================================

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <GyverPortal.h>

#include <EEManager.h>

// #include "softUART.h"
// #include "GBUS.h"
// softUART<0, GBUS_FULL> UART(1000);
// GBUS bus(&UART, 5, 20);

#define GT_STREAM_MODE // STREAM_MODE нужен для работы GBUS
#include <GyverTransfer.h>
#include "GBUS.h"
GyverTransfer<0, GT_TRX> trans;
GBUS bus(&trans, MY_ADDRESS, BUFFER); // адрес 5, буфер 20 байт

byte temp[4];
byte temp_max[6];
bool led_state[4];
bool relay_state[6];

struct Data_resieve
{
  bool relay_state1;
  bool relay_state2;
  bool relay_state3;
  bool relay_state4;
  bool relay_state5;

  byte temp1;
  byte temp2;
  byte temp3;
  byte temp4;
};

struct Data_transmit
{
  bool relay_state6;
  byte temp_max1;
  byte temp_max2;
  byte temp_max3;
  byte temp_max4;
  byte temp_max5;
  byte temp_max6;
};

GyverPortal portal;
EEManager mem1(temp_max);

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

  Serial.begin(9600);
  Serial.println("Start");

  Serial.println(temp_max[0]);
  Serial.println(temp_max[1]);
  Serial.println(temp_max[2]);
  Serial.println(temp_max[3]);
  Serial.println(temp_max[4]);
  Serial.println(temp_max[5]);

  CONNECT_PORTAL();
  CONNECT_WIFI();

  pinMode(LED_PIN, 1);
  digitalWrite(LED_PIN, 0);
  attachInterrupt(0, isr, CHANGE);
}

IRAM_ATTR void isr() { trans.tickISR(); }

void loop()
{
  portal.tick();
  mem1.tick();
  bus.tick();

  static uint32_t timer1;
  if (millis() - timer1 >= 500) // мигалка
  {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    timer1 = millis();
  }

  static uint32_t timer2;
  if (millis() - timer2 >= 1000) // отправка запроса
  {
    timer2 = millis();
    Serial.println("send request");
    bus.sendRequest(3);
  }

  if (bus.gotRequest())
  {
    Serial.print("Got request: ");
    // bus.sendAck(bus.getTXaddress());
    Serial.println(bus.getTXaddress());

    Data_transmit data_tr;

    data_tr.relay_state6 = relay_state[5];
    data_tr.temp_max1 = temp_max[0];
    data_tr.temp_max2 = temp_max[1];
    data_tr.temp_max3 = temp_max[2];
    data_tr.temp_max4 = temp_max[3];
    data_tr.temp_max5 = temp_max[4];
    data_tr.temp_max6 = temp_max[5];

    bus.sendData(bus.getTXaddress(), data_tr);
  }

  // Метод waitAck(адрес, кол-во попыток, таймаут)
  byte state = bus.waitAck(3, 5, 500);
  // Serial.println(state);
  switch (state)
  {
  case ACK_ERROR:
    Serial.println("ack error");
    break;

  case ACK_ONLY: // отправляем данные
    Serial.println("got ack");
    break;

  case ACK_DATA: // получение данных
    Serial.println("got data: ");
    Data_resieve data_rs;
    bus.readData(data_rs);

    temp[0] = data_rs.temp1;
    temp[1] = data_rs.temp2;
    temp[2] = data_rs.temp3;
    temp[3] = data_rs.temp4;
    relay_state[0] = data_rs.relay_state1;
    relay_state[1] = data_rs.relay_state2;
    relay_state[2] = data_rs.relay_state3;
    relay_state[3] = data_rs.relay_state4;
    relay_state[4] = data_rs.relay_state5;

    for (byte i = 0; i < 4; i++)
      Serial.println(temp[i]);
    for (byte i = 0; i < 5; i++)
      Serial.println(relay_state[i]);

    if (temp[2] < temp_max[5]) // sensor 3
      led_state[2] = 0;
    else
      led_state[2] = 1;
    break;
  }
}