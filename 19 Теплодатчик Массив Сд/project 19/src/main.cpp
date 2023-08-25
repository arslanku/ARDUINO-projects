#include <Arduino.h>
#include <GParser.h>
#include <Adafruit_MLX90614.h>
#include <SPI.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup()
{
  Serial.begin(9600);
  Serial.println("Start");

  mlx.begin();
}

void getTemp(byte data_x, byte data_y)
{
  byte sensorData[data_x];
  for (byte y = 0; y < data_y; y++)
  {
    for (byte x = 0; x < data_x; x++)
    {
      if (y % 2 == 0)
        sensorData[x] = mlx.readObjectTempC();
      else
        sensorData[data_x - x - 1] = mlx.readObjectTempC();
    }
    for (byte x = 0; x < data_x; x++)
    {
      Serial.print(sensorData[x]);
      Serial.print(" ");
      delay(1);
    }
    Serial.println("");
  }
}

void loop()
{
  if (Serial.available())
  {
    char strData[30];
    byte amount = Serial.readBytesUntil(';', strData, 30);
    strData[amount] = '\0';

    GParser data(strData, ',');

    byte coords[data.amount()];
    data.parseBytes(coords);

    for (byte i = 0; i < data.amount(); i++)
    {
      Serial.println(coords[i]);
      delay(10);
    }

    getTemp(coords[0], coords[1]);
  }
}
