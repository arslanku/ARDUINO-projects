В#include <Wire.h>
#include "AHTxx.h"

float ahtValue;

AHTxx aht10(AHTXX_ADDRESS_X38, AHT1x_SENSOR);

void printStatus()
{
  switch (aht10.getStatus())
  {
  case AHTXX_NO_ERROR:
    Serial.println(F("Ок"));
    break;

  case AHTXX_BUSY_ERROR:
    Serial.println(F("Датчик занят, увеличьте время опроса"));
    break;

  case AHTXX_ACK_ERROR:
    Serial.println(F("Датчик не вернул подтверждение (не подключен, сломан, длинные провода, шина заблокирована ведомым устройством)"));
    break;

  case AHTXX_DATA_ERROR:
    Serial.println(F("Полученные данные меньше ожидаемых (не подключен, оборван, длинные провода, шина заблокирована ведомым устройством)"));
    break;

  case AHTXX_CRC8_ERROR:
    Serial.println(F("Вычисленный CRC8 не соответствует полученному CRC8, эта функция поддерживается только датчиками AHT2x"));
    break;

  default:
    Serial.println(F("Неопределенный статус"));
    break;
  }
}

void TCA9548A(uint8_t bus)
{
  Wire.beginTransmission(0x70);
  Wire.write(1 << bus);
  Wire.endTransmission();
}

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  for (byte i = 0; i <= 7; i++)
  {
    TCA9548A(i);

    if (aht10.begin())
    {
      Serial.println("Датчик AHT №" + i);
      Serial.println("подключен успешно");
    }
    else
    {
      Serial.println("Ошибка подключения датчика AHT №" + i);
      delay(100);
    }
  }
}

void loop()
{
  for (byte i = 0; i <= 7; i++)
  {
    TCA9548A(i);

    ahtValue = aht10.readTemperature();

    Serial.print(F("Температура: "));

    if (ahtValue != AHTXX_ERROR)
    {
      Serial.print(ahtValue);
      Serial.println(F(" +-0.3C"));
    }
    else
    {
      printStatus();
    }

    ahtValue = aht10.readHumidity(AHTXX_USE_READ_DATA);

    Serial.print(F("Влажность: "));

    if (ahtValue != AHTXX_ERROR)
    {
      Serial.print(ahtValue);
      Serial.println(F(" +-2%"));
    }
    else
    {
      printStatus();
    }

    delay(255);
  }
}
