#include <Arduino.h>

// ==============================================

#define LCD_SDA_PIN A4
#define LCD_SCL_PIN A5

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// LiquidCrystal_I2C lcd(0x3f, 20, 4);
LiquidCrystal_I2C lcd(0x27, 20, 4);

// ==============================================

#define DS_SENSOR_AMOUNT 7
#define DS_SENSOR_PIN 3

uint8_t addr[][8] = {
    {0x28, 0x6E, 0x29, 0x57, 0x4, 0xE1, 0x3C, 0xA9},
    {0x28, 0x10, 0xDE, 0x57, 0x4, 0xE1, 0x3C, 0xD5},
    {0x28, 0x8E, 0x60, 0x57, 0x4, 0xE1, 0x3C, 0x9A},
    {0x28, 0xA6, 0xC9, 0x57, 0x4, 0xE1, 0x3C, 0x4D},
    {0x28, 0xA1, 0x82, 0x57, 0x4, 0xE1, 0x3C, 0x51},
    {0x28, 0x90, 0xF6, 0x57, 0x4, 0xE1, 0x3C, 0xF9},
    {0x28, 0xFF, 0x64, 0x18, 0x2D, 0xBA, 0x70, 0xFB}};
#include <microDS18B20.h>

MicroDS18B20<DS_SENSOR_PIN, DS_ADDR_MODE> sensor[DS_SENSOR_AMOUNT];

void setup()
{
  pinMode(13, 1);

  pinMode(8, 1);
  digitalWrite(8, 1);

  pinMode(A0, 1);
  digitalWrite(A0, 1);

  pinMode(A1, 1);
  digitalWrite(A1, 1);

  pinMode(A2, 1);
  digitalWrite(A2, 1);

  pinMode(A3, 1);
  digitalWrite(A3, 1);

  Serial.begin(9600);

  for (int i = 0; i < DS_SENSOR_AMOUNT; i++)
    sensor[i].setAddress(addr[i]);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("hello!");
}

void loop()
{
  static uint32_t tmr;
  if (millis() - tmr >= 1000)
  {
    tmr = millis();
    lcd.clear();

    float temp[7] = {};
    for (int i = 0; i < DS_SENSOR_AMOUNT; i++)
    {
      temp[i] = sensor[i].getTemp();
      //      temp[i] = i + 10;
      sensor[i].requestTemp();

      Serial.print(temp[i]);
      Serial.print(" ");
    }
    Serial.println();

    lcd.setCursor(0, 0);
    for (byte i = 0; i < 4; i++)
    {
      int int_temp = temp[i];
      lcd.print(int_temp);
      lcd.print(" ");
    }

    lcd.setCursor(0, 1);
    for (byte i = 4; i < 7; i++)
    {
      int int_temp = temp[i];
      lcd.print(int_temp);
      lcd.print(" ");
    }
  }

  static uint32_t tmr_led;
  if (millis() - tmr_led >= 500)
  {
    tmr_led = millis();
    digitalWrite(13, !digitalRead(13));
  }
}