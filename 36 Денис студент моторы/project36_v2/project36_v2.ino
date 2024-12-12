#include <Arduino.h>

// ==================================================================
// НАСТРОЙКИ РАБОТЫ ПРОГРАММЫ

#define SENSOR1_PIN A0 // датчик давления 1 на пин A0
#define SENSOR2_PIN A1 // датчик давления 2 на пин A1
#define SENSOR3_PIN A2 // датчик давления 3 на пин A2
#define SENSOR4_PIN A3 // датчик давления 4 на пин A3

#define PWMA 11 // ШИМ пин для подключения двигателя 1
#define AIN1 7  // управляюшие выводы для двигателя 1
#define AIN2 6  // управляюшие выводы для двигателя 1

#define PWMB 10 // ШИМ пин для подключения двигателя 2
#define BIN1 5  // управляюшие выводы для двигателя 2
#define BIN2 4  // управляюшие выводы для двигателя 2

#define STBY 13 // пин для запуска драйвера двигетелей

#define SENSOR_LOW 100  // нижний порог значений с датчика (от 0 до 1023)
#define SENSOR_HIGH 400 // верхний порог значений с датчика (от 0 до 1023)

int sensor1; // переменная, для храния значения с датчика 1
int sensor2; // переменная, для храния значения с датчика 2
int sensor3; // переменная, для храния значения с датчика 3
int sensor4; // переменная, для храния значения с датчика 4

#define START_WORKING_DAY 7   // работаем с 7:00 утра
#define FINISH_WORKING_DAY 22 // до 22:00 вечера

#define SLEEP_TIME 10800000 // время сна (В МИЛЛИСЕКУНДАХ)

// ==================================================================
// ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ ДЛЯ РАБОТЫ ПРОГРАММЫ

int eeprom_addres;                   // переменная для адреса в EEPROM, по которому будем записывать данные с датчиков
bool working_time;                   // переменная для проверки рабочего периода времени (с 7:00 до 22:00)
bool motor1_running, motor2_running; // переменные для текущего состояния работы двигателей

// структура для хранения данных с датчиков
struct Data
{
  byte day_and_hour[2]; // массив: календарный день и час
  int sensors_value[4]; // массив: данные с 4-х датчиков
} data;

// ==================================================================
// ПОДКЛЮЧЕНИЕ БИБЛИОТЕК

// для работы с часами реального времени
#include <microDS3231.h>
MicroDS3231 rtc;

// для управления сном
#include <GyverPower.h>

// для работы с энергонезависимой памятью (EEPROM)
#include <EEPROM.h>

// ==================================================================
// ФУНКЦИИ РАБОТЫ С МОТОРАМИ

void motor1_forward() // функция: движение вперёд мотор 1
{
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 255);
}

void motor2_forward() // функция: движение вперёд мотор 2
{
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 255);
}

void motor1_backward() // функция: движение назад мотор 1
{
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, 255);
}

void motor2_backward() // функция: движение назад мотор 2
{
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, 255);
}

void motor1_off() // функция: тормоз, холостой ход мотор 1
{
  digitalWrite(AIN1, HIGH); // тормоз
  digitalWrite(AIN2, HIGH);

  digitalWrite(AIN1, LOW); // холостой ход, стоим
  digitalWrite(AIN2, LOW);

  analogWrite(PWMA, 0);
}

void motor2_off() // функция: тормоз, холостой ход мотор 2
{
  digitalWrite(BIN1, HIGH); // тормоз
  digitalWrite(BIN2, HIGH);

  digitalWrite(BIN1, LOW); // холостой ход, стоим
  digitalWrite(BIN2, LOW);

  analogWrite(PWMB, 0);
}

// ==================================================================
// ОСНОВНЫЕ ФУНКЦИИ SETUP, LOOP

void setup()
{
  Serial.begin(9600); // скорость обмена данными с компьютером

  // установка пинов на режим вход (для датчиков)
  pinMode(SENSOR1_PIN, INPUT);
  pinMode(SENSOR2_PIN, INPUT);
  pinMode(SENSOR3_PIN, INPUT);
  pinMode(SENSOR4_PIN, INPUT);

  // установка пинов на режим выход (для моторов)
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(STBY, OUTPUT);

  // включаем драйвер двигателей
  digitalWrite(STBY, HIGH);

  // ВНИМАНИЕ!!!
  // калибровка таймера для сна
  int max_timeout = power.getMaxTimeout();
  power.calibrate(max_timeout); // потом сюда записать полученное значение из переменной max_timeout. Затем удалить верхнюю и нижнюю строчку
  Serial.println(max_timeout);

  // настройка режима сна
  power.setSleepMode(EXTSTANDBY_SLEEP);

  // проверка наличия модуля ds3231 на линии i2c
  if (rtc.begin())
  {
    Serial.println("[ИНФО] DS3231 подключен успшешно");
  }
  else
  {
    Serial.println("[ОШИБКА] DS3231 не подключен");
  }

  // установить время равное времени компиляции программы (берем время с компьютера)
  rtc.setTime(COMPILE_TIME);

  // читаем и выводим, с помощью цикла, данные, которые уже есть в памяти
  Serial.println("[ИНФО] Чтение данных из EEPROM памяти:");
  for (int i = 0; i < 100; i++)
  {
    EEPROM.get(i * 10, data);
    Serial.print("Календарный день: ");
    Serial.print(data.day_and_hour[0]);
    Serial.print(" | час: ");
    Serial.print(data.day_and_hour[1]);
    Serial.print(" - ");
    Serial.print(data.sensors_value[0]);
    Serial.print(" - ");
    Serial.print(data.sensors_value[1]);
    Serial.print(" - ");
    Serial.print(data.sensors_value[2]);
    Serial.print(" - ");
    Serial.println(data.sensors_value[3]);
  }
  Serial.println("[ИНФО] Чтение данных завершено");
}

void loop()
{
  // чтение значений с датчиков и запись значений в переменные
  sensor1 = analogRead(SENSOR1_PIN);
  sensor2 = analogRead(SENSOR2_PIN);
  sensor3 = analogRead(SENSOR3_PIN);
  sensor4 = analogRead(SENSOR4_PIN);

  // получаем текущее время и записываем его в объект класса DateTime
  DateTime now = rtc.getTime();

  // ПРОВЕРКА РАБОЧЕГО ВРЕМЕНИ ======================================

  // если сейчас между 7 утра и 22 вечера - то рабочее время (есть возможность работы двигателей),
  // иначе - нерабочее время (двигатели не смогут работать)
  if (now.hour >= START_WORKING_DAY and now.hour < FINISH_WORKING_DAY)
  {
    working_time = true;
  }
  else
  {
    working_time = false;
  }

  // ПЕРВЫЙ МОТОР ===================================================

  if (
      ((SENSOR_LOW < sensor1 and sensor1 < SENSOR_HIGH) and sensor2 <= SENSOR_LOW) or
      ((SENSOR_LOW < sensor2 and sensor2 < SENSOR_HIGH) and sensor1 <= SENSOR_LOW))
  {
    // Если хотя бы одно значение от Датчика1 или Датчика2 равно В,
    // а другое значение (с Датчика2 или с Датчика1) равно или меньше значения В,
    // то ничего не делаем и не трогаем мотор-редуктор1.

    // выключаем мотор 1
    motor1_off();
    motor1_running = false;
  }
  else if (
      (sensor1 < SENSOR_LOW and sensor2 < SENSOR_LOW) and working_time)
  {
    // Если оба значения с датчиков 1 и 2 меньше значения В,
    // то включаем мотор-редуктор1 против часовой стрелки, чтобы замотать
    // трос и увеличить давление до нужного.
    // Так же проверяем рабочий ли сейчас промежуток времени

    // включаем мотор 1 против часовой стрелки
    motor1_backward();
    motor1_running = true;
  }
  else if (
      (sensor1 > SENSOR_HIGH or sensor2 > SENSOR_HIGH) and working_time)
  {
    // Если хотя бы один из значений датчиков 1 или 2 больше значения В,
    // то включаем мотор-редуктор1 по часовой стрелки, чтобы размотать
    // трос и уменьшить давление до нужного.
    // Так же проверяем рабочий ли сейчас промежуток времени

    // включаем мотор 2 по часовой стрелке
    motor1_forward();
    motor1_running = true;
  }

  // ВТОРОЙ МОТОР ===================================================

  if (
      ((SENSOR_LOW < sensor3 and sensor3 < SENSOR_HIGH) and sensor4 <= SENSOR_LOW) or
      ((SENSOR_LOW < sensor4 and sensor4 < SENSOR_HIGH) and sensor3 <= SENSOR_LOW))
  {
    // Если хотя бы одно значение от Датчика3 или Датчика4 равно В,
    // а другое значение (с Датчика3 или с Датчика4) равно или меньше значения В,
    // то ничего не делаем и не трогаем мотор-редуктор2.

    // выключаем мотор 2
    motor2_off();
    motor2_running = false;
  }
  else if (
      (sensor3 < SENSOR_LOW and sensor4 < SENSOR_LOW) and working_time)
  {
    // Если оба значения с датчиков 3 и 4 меньше значения В,
    // то включаем мотор-редуктор2 против часовой стрелки, чтобы замотать
    // трос и увеличить давление до нужного.
    // Так же проверяем рабочий ли сейчас промежуток времени

    // включаем мотор 2 против часовой стрелки
    motor2_backward();
    motor2_running = true;
  }
  else if (
      (sensor3 > SENSOR_HIGH or sensor4 > SENSOR_HIGH) and working_time)
  {
    // Если хотя бы один из значений датчиков 3 или 4 больше значения В,
    // то включаем мотор-редуктор2 по часовой стрелки, чтобы размотать
    // трос и уменьшить давление до нужного.
    // Так же проверяем рабочий ли сейчас промежуток времени

    // включаем мотор 2 по часовой стрелке
    motor2_forward();
    motor2_running = true;
  }

  // проверяем, что двигатели отработали и готовимся ко сну
  if (!motor1_running && !motor2_running)
  {
    // ЗАПИСЬ ДАННЫХ В EEPROM =========================================

    // получаем адрес для записи данных с датчиков
    EEPROM.get(1010, eeprom_addres);

    data.day_and_hour[0] = now.day;  // получаем календарный день
    data.day_and_hour[1] = now.hour; // получаем час

    // записываем значения с датчиков в структуру данных
    data.sensors_value[0] = sensor1;
    data.sensors_value[1] = sensor2;
    data.sensors_value[2] = sensor3;
    data.sensors_value[3] = sensor4;

    // вывод значений с датчиков на компьютер
    Serial.println("[ИНФО] Двигатели отработали");
    Serial.println("[ИНФО] Текущие значения с датчиков:");
    Serial.print("Календарный день: ");
    Serial.print(data.day_and_hour[0]);
    Serial.print(" | час: ");
    Serial.print(data.day_and_hour[1]);
    Serial.print(" - ");
    Serial.print(data.sensors_value[0]);
    Serial.print(" - ");
    Serial.print(data.sensors_value[1]);
    Serial.print(" - ");
    Serial.print(data.sensors_value[2]);
    Serial.print(" - ");
    Serial.println(data.sensors_value[3]);

    // записываем данные с датчиков в EEPROM память
    EEPROM.put(eeprom_addres, data);
    // обновляем адрес для записи в  EEPROM
    eeprom_addres += 10;
    // сбрасываем адрес, если дошли до конца памяти
    if (eeprom_addres >= 1000)
    {
      eeprom_addres = 0;
    }
    // записываем новый адрес в EEPROM
    EEPROM.put(eeprom_addres, 1010);
    Serial.println("[ИНФО] Данные записаны в EEPROM");

    // спим 3 часа
    power.sleepDelay(SLEEP_TIME);
  }
}
