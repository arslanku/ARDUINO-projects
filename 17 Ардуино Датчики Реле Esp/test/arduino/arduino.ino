// отправка данных по однопроводному юарту

// подключаем софт юарт
#include "softUART.h"
// делаем только отправителем (экономит память)
softUART<4, GBUS_TX> UART(1000); // пин 4, скорость 1000

// подключаем GBUS
#include "GBUS.h"
GBUS bus(&UART, 3, 20); // обработчик UART, адрес 3, буфер 20 байт

struct myStruct {
  float val_f;
  float val_f2;
  long val_i;
  long val_l;
  byte val_b;
};

void setup() {
  Serial.begin(9600);
  pinMode(13, 1);
}


void loop() {
  //  Serial.println("loop");
  bus.tick();

  static uint32_t timer;
  if (millis() - timer >= 500) {
    digitalWrite(13, !digitalRead(13));
    timer = millis();
  }

  static uint32_t tmr;
  if (millis() - tmr >= 2000) {
    tmr = millis();
    Serial.println("send request");
    bus.sendRequest(5);
  }

  byte state = bus.waitAck(5, 5, 500);
  // Serial.println(state);
  switch (state)
  {
    case ACK_ERROR:
      Serial.println("ack error");
      break;

    case ACK_ONLY: // отправляем данные
      Serial.println("got ack. send data");

      myStruct data;
      data.val_f = 12.34;
      data.val_f2 = 56.78;
      data.val_i = 1234;
      data.val_l = 123456;
      data.val_b = 12;
      bus.sendData(5, data);
      break;
  }
}
