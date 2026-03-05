// Подключаем библиотеку Arduino
#include <Arduino.h>

// Подключаем библиотеку Wire
#include <Wire.h>

// Подключаем библиотеку BH1750 для работы с датчиком освещенности
#include <BH1750.h>

// Создаем объект lightMeter_GY30 для взаимодействия с библиотекой
BH1750 lightMeter_GY30;

// Тип подключения дисплея: 1 - по шине I2C, 2 - десятиконтактное
#define _LCD_TYPE 1

// Подключаем библиотеку LiquidCrystal_I2C для работы с дисплеем
#include <LiquidCrystal_I2C.h>

// Создаеи объект lcd для взаимодействия с библиотекой
LiquidCrystal_I2C lcd(0x27, 16, 2);

// создаем переменную с номером пина для мосфет-модуля
int mosfetPin = 5;

// нужный уровень яркости (в Люксах)
int desiredBrightnessLevel = 450;

// максимальный уровень яркости (в Люксах)
int maximumBrightnessLevel = 54000;

void setup()
{
  // Открываем последовательную связь с ПК на скорости 9600 бод
  Serial.begin(9600);

  // Подключение протокола i2c
  Wire.begin();

  // Инициализация библиотеки BH1750
  lightMeter_GY30.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23);

  // Инициализация дислплея
  lcd.init();

  // Включение подсветки на дисплее
  lcd.backlight();

  // Устанавливаем пин на режим "ВЫХОД"
  pinMode(mosfetPin, OUTPUT);
}

// переменная для хренения значения с датчика освещенности
float lux = 0;

// переменная для хранения значения яркости лампочки (уровень ШИМ сигнала)
// int brightness = 0;
int brightness = desiredBrightnessLevel * 1024 / maximumBrightnessLevel;

// переменная для хранения текста, который будет выводиться на дисплей
String text = "";

void loop()
{
  // считываем показания с датчика в переменную lux
  lux = lightMeter_GY30.readLightLevel();

  // если уровень яркости меньше нужного, включаем яркость на 100%
  if (lux < desiredBrightnessLevel)
  {
    analogWrite(mosfetPin, brightness);
  }
  // если уровень яркости больше нужного на 25%, включаем яркость 75%
  else if (lux > desiredBrightnessLevel * 1.25)
  {
    analogWrite(mosfetPin, brightness * 0.75);
  }
  // если уровень яркости больше нужного на 50%, включаем яркость 50%
  else if (lux > desiredBrightnessLevel * 1.5)
  {
    analogWrite(mosfetPin, brightness * 0.5);
  }
  // если уровень яркости больше нужного на 100% (т.е. в 2 раза), выключаем светодиод
  else if (lux > desiredBrightnessLevel * 2)
  {
    analogWrite(mosfetPin, LOW);
  }

  // упаковываем данные в переменную
  text = "LUX: " + String(lux);

  // чистим дисплей от предыдущих значений
  lcd.clear();

  // выводим новые данные
  lcd.print(text);

  delay(100);
}
