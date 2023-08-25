#include <Wire.h>
#include "Adafruit_AHT10.h"

Adafruit_AHT10 aht;

void TCA9548A(uint8_t bus) {
  Wire.beginTransmission(0x70);
  Wire.write(1 << bus);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  for (byte i = 0; i <= 7; i++) {
    TCA9548A(i);

    if (aht.begin()) {
      Serial.println("Датчик AHT №" + i);
      Serial.println("подключен успешно");
    }
    else {
      Serial.println("Ошибка подключения датчика AHT №" + i);
      delay(100);
    }
  }
}

void loop() {
  for (byte i = 0; i <= 7; i++) {
    TCA9548A(i);

    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);

    Serial.println("Датчик №" + i);
    Serial.print("Температура: "); Serial.print(temp.temperature); Serial.println(" C");
    Serial.print("Влажность: "); Serial.print(humidity.relative_humidity); Serial.println(" % \n");

    delay(500);
  }
}
