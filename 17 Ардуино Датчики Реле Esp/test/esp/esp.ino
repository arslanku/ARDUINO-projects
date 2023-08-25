// приём данных по однопроводному юарту

// подключаем софт юарт
#include "softUART.h"
// делаем только приёмником (экономит память)
softUART<0, GBUS_RX> UART(1000); // пин 4, скорость 1000

// подключаем GBUS
#include "GBUS.h"
GBUS bus(&UART, 5, 20); // обработчик UART, адрес 5, буфер 20 байт

struct myStruct {
  float val_f;
  float val_f2;
  long val_i;   // а тут long, иначе всё сломается
  long val_l;
  byte val_b;
};

void setup() {
  Serial.begin(115200); // сериал для отладки (вывод в монитор)
  pinMode(2, 1);
}

void loop() {
  // в тике сидит отправка и приём
  bus.tick();

  static uint32_t t;
  if (millis() - t >= 500) {
    digitalWrite(2, !digitalRead(2));
    t = millis();
  }

  if (bus.gotRequest())
  {
    Serial.println("Got request");
    //    bus.sendAck(bus.getTXaddress());
    Serial.println(bus.getTXaddress());
  }

  if (bus.gotData()) {
    // выводим данные
    Serial.println("got data!");
    myStruct data;
    bus.readData(data);

    Serial.println(data.val_f);
    Serial.println(data.val_f2);
    Serial.println(data.val_i);
    Serial.println(data.val_l);
    Serial.println(data.val_b);
    Serial.println();
  }
}
