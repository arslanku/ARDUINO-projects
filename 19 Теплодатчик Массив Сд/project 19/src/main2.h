

#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <SPI.h>
#include <SD.h>
#include <ATtinySerialOut.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

int temperature = 0;

int numRows = 15;
int numColumns = 15;
int sensorData[numRows][numColumns];

int temp = 0;
int getTemp()
{
    temp++;
    return temp;
}

// byte sensorData[data_x][data_y];
// byte **sensorData = new byte *[data_y];
// for (byte i = 0; i < data_x; i++)
//   sensorData[i] = new byte[data_x];

// for (byte y = 0; y < data_y; y++)
// {
//   for (byte x = 0; x < data_x; x++)
//   {
//     if (y % 2 == 0)
//       sensorData[x][y] = readObjectTempC();
//     else
//       sensorData[data_x - x - 1][y] = readObjectTempC();
//     // Serial.println(sensorData[x][y]);
//   }
//   for (byte x = 0; x < data_x; x++)
//   {
//     Serial.print(sensorData[x][y]);
//     Serial.print(" ");
//     sensorData[x][y] = NULL;
//     // delay(5);
//   }
//   Serial.println("");
// }

void setup()
{
    Serial.begin(9600);
    mlx.begin();
    delay(3000);

    // temperature = mlx.readObjectTempC()
    for (int y = 0; y < numColumns; y++)
    {
        for (int x = 0; x < numRows; x++)
        {
            if (y % 2 == 0)
                sensorData[x][y] = getTemp();
            else
                sensorData[numRows - x - 1][y] = getTemp();
        }
    }

    // for (int x = 0; i < numRows; x++)
    // {
    //   for (int y = 0; j < numColumns; y++)
    //   {
    //     // читаем данные

    //     // записываем в ячейку массива
    //     sensorData[x][y] = temperature;

    //     // отправляем данные из ячейки массива в сериал порт
    //     Serial.println(sensorData[x][y]);
    //   }
    // }

    for (int y = 0; y < numColumns; y++)
    {
        for (int x = 0; x < numRows; x++)
        {
            Serial.print(sensorData[x][y]);
            Serial.print(" ");
        }
        Serial.println("");
    }
}

void loop()
{
}