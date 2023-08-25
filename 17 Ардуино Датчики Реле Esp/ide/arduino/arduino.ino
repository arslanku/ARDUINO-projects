#define SENSOR_PIN1 A0
#define SENSOR_PIN2 A2
#define SENSOR_PIN3 A3
#define SENSOR_PIN4 A5

#define RELAY_PIN1 2
#define RELAY_PIN2 3
#define RELAY_PIN3 4
#define RELAY_PIN4 5
#define RELAY_PIN5 6
#define RELAY_PIN6 8

#define GBUS_PIN 2

#define INTERVAL 1000

#define MY_ADDRESS 3
#define BUFFER 40

#include <Arduino.h>
#include "GyverWDT.h"

// #include "libdeps/microDS18B20/src/microDS18B20.h"
#include <microDS18B20.h>
MicroDS18B20<SENSOR_PIN1> sensor1;
MicroDS18B20<SENSOR_PIN2> sensor2;
MicroDS18B20<SENSOR_PIN3> sensor3;
MicroDS18B20<SENSOR_PIN4> sensor4;

// #include "libdeps/GyverBus/src/softUART.h"
// #include "libdeps/GyverBus/src/GBUS.h"
// #include <softUART.h>
// #include <GBUS.h>
// softUART<10, GBUS_FULL> UART(1000);
// GBUS bus(&UART, 3, 20);

#define GT_STREAM_MODE // STREAM_MODE нужен для работы GBUS
#include <GyverTransfer.h>
#include "GBUS.h"
GyverTransfer<GBUS_PIN, GT_TRX> trans;
GBUS bus(&trans, MY_ADDRESS, BUFFER); // адрес 3, буфер 20 байт

byte temp[4];
byte temp_max[6] = {30, 30, 30, 30, 30, 30};
bool relay_state[6];
bool led_state[4];
byte errors;

struct Data_resieve
{
  bool relay_state6;
  byte temp_max1;
  byte temp_max2;
  byte temp_max3;
  byte temp_max4;
  byte temp_max5;
  byte temp_max6;
};

struct Data_transmit
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

void setup()
{
  Serial.begin(9600);
  Serial.println("Start");
  Watchdog.enable(RST_MODE, WDT_TIMEOUT_2S);

  pinMode(13, 1);
  // restart_esp();

  randomSeed(analogRead(A1));
  attachInterrupt(0, isr, CHANGE);
}

void isr() { trans.tickISR(); }

void restart_esp() // функций для перезагрузки есп
{
  // digitalWrite(8, 0);
  Serial.println("[RESTART ESP] state: BEGIN");
  pinMode(8, INPUT);
  digitalWrite(8, 0);
  pinMode(8, OUTPUT);
  digitalWrite(8, 0);
  delay(10);
  pinMode(8, INPUT);
  Serial.println("[RESTART ESP] state: OK");
  errors = 0;
}

void loop()
{
  bus.tick();
  Watchdog.reset();

  if (errors >= 4)
    restart_esp();

  static uint32_t timer1;
  if (millis() - timer1 >= 500) // мигалка
  {
    digitalWrite(13, !digitalRead(13));
    timer1 = millis();

    // digitalWrite(RELAY_PIN1, relay_state[0]);
    // digitalWrite(RELAY_PIN2, relay_state[1]);
    // digitalWrite(RELAY_PIN3, relay_state[2]);
    // digitalWrite(RELAY_PIN4, relay_state[3]);
    // digitalWrite(RELAY_PIN5, relay_state[4]);
    // digitalWrite(RELAY_PIN6, relay_state[5]);
  }

  static uint32_t timer2;
  if (millis() - timer2 >= 1000) // отправка запроса
  {
    timer2 = millis();
    Serial.print("errors: ");
    Serial.println(errors);

    Serial.println("send request");
    bus.sendRequest(5);
  }

  if (bus.gotRequest()) // отправка данных
  {
    Serial.print("Got request: ");
    // bus.sendAck(bus.getTXaddress());
    Serial.println(bus.getTXaddress());

    Data_transmit data_tr;

    temp[0] = random(20, 80);
    temp[1] = random(20, 80);
    temp[2] = random(20, 80);
    temp[3] = random(20, 80);

    data_tr.temp1 = temp[0];
    data_tr.temp2 = temp[1];
    data_tr.temp3 = temp[2];
    data_tr.temp4 = temp[3];

    data_tr.relay_state1 = relay_state[0];
    data_tr.relay_state2 = relay_state[1];
    data_tr.relay_state3 = relay_state[2];
    data_tr.relay_state4 = relay_state[3];
    data_tr.relay_state5 = relay_state[4];

    bus.sendData(bus.getTXaddress(), data_tr);
  }

  if (millis() > 2000) // изменение положений реле
  {
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
  }

  // Метод waitAck(адрес, кол-во попыток, таймаут)
  byte state = bus.waitAck(5, 5, 500);
  // Serial.println(state);
  switch (state)
  {
  case ACK_ERROR:
    Serial.println("ack error");
    errors++;
    break;

  case ACK_ONLY:
    Serial.println("got ack");
    errors--;
    break;

  case ACK_DATA: // получение данных
    Data_resieve data_rs;
    Serial.println("got data:");
    bus.readData(data_rs);
    errors = 0;

    relay_state[5] = data_rs.relay_state6;
    temp_max[0] = data_rs.temp_max1;
    temp_max[1] = data_rs.temp_max2;
    temp_max[2] = data_rs.temp_max3;
    temp_max[3] = data_rs.temp_max4;
    temp_max[4] = data_rs.temp_max5;
    temp_max[5] = data_rs.temp_max6;

    Serial.println(relay_state[5]);
    for (byte i = 0; i < 6; i++)
      Serial.println(temp_max[i]);
    break;
  }
}
