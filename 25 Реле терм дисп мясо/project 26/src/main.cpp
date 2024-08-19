#include <Arduino.h>

// ====================================

#define RELAY1_PIN 2
#define RELAY2_PIN 3
#define RELAY3_PIN 4
#define RELAY4_PIN 5
#define RELAY5_PIN 6
#define RELAY6_PIN 7

bool relayState[] = {0, 0, 0, 0, 0, 0};
byte relayPins[] = {RELAY1_PIN, RELAY2_PIN, RELAY3_PIN,
                    RELAY4_PIN, RELAY5_PIN, RELAY6_PIN};

byte relay[2][6] = {
    {RELAY1_PIN, RELAY2_PIN, RELAY3_PIN, RELAY4_PIN, RELAY5_PIN, RELAY6_PIN},
    {0, 0, 0, 0, 0, 0},
};

// ====================================

#define BTN1_PIN A0
#define BTN2_PIN A1
#define BTN3_PIN A2
#define BTN4_PIN 11
#define BTN5_PIN 12

#include <EncButton.h>
Button btn1(BTN1_PIN);
Button btn2(BTN2_PIN);
Button btn3(BTN3_PIN);
Button btn4(BTN4_PIN);
Button btn5(BTN5_PIN);

// ====================================

#define SENSOR_PIN 8
#define DS_SENSOR_AMOUNT 3

#include <microDS18B20.h>

float temp[3];
uint8_t addr[][8] = {
    {0x28, 0xFF, 0x78, 0x5B, 0x50, 0x17, 0x4, 0xCF},
    {0x28, 0xFF, 0x99, 0x80, 0x50, 0x17, 0x4, 0x4D},
    {0x28, 0xFF, 0x53, 0xE5, 0x50, 0x17, 0x4, 0xC3},
};

MicroDS18B20<SENSOR_PIN, DS_ADDR_MODE> sensor[DS_SENSOR_AMOUNT];

// ====================================

#define LCD_SDA_PIN A4
#define LCD_SCL_PIN A5
// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>
// LiquidCrystal_I2C lcd(0x3f, 20, 4);
// LiquidCrystal_I2C lcd(0x27, 20, 4);
#define _LCD_TYPE 1
#include <LCD_1602_RUS_ALL.h>
LCD_1602_RUS lcd(0x27, 20, 4);
byte menu = 0;

// ====================================

#include <EEManager.h>
struct Data
{
  byte temp1_min;
  byte temp1_max;
  byte temp2;
} data;
EEManager mem(data);

// ====================================

#include "display.h"
#include "buttons.h"
#include "tempSensors.h"

// ====================================

void setup()
{
  Serial.begin(9600);

  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  pinMode(RELAY4_PIN, OUTPUT);
  pinMode(RELAY5_PIN, OUTPUT);

  mem.begin(0, 'a');
  Serial.println(data.temp1_min);
  Serial.println(data.temp1_max);
  Serial.println(data.temp2);

  for (int i = 0; i < DS_SENSOR_AMOUNT; i++)
    sensor[i].setAddress(addr[i]);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  display();
}

void loop()
{
  mem.tick();

  btn1.tick();
  btn2.tick();
  btn3.tick();
  btn4.tick();
  btn5.tick();

  buttons();
  updTemp();
}
