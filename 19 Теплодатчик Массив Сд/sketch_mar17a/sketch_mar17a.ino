#include <SoftwareSerial.h>
SoftwareSerial scannerSerial(7, 8);

void setup() {
  Serial.begin(9600);
  scannerSerial.begin(9600);
}

void loop() {
  if (scannerSerial.available())
  {
    uint32_t data = scannerSerial.parseInt();
    Serial.println(data);
  }
}
