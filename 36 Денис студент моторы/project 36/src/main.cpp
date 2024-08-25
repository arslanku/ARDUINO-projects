#include <Arduino.h>

// ==================================================================
// НАСТРОЙКИ РАБОТЫ ПРОГРАММЫ

#define SENSOR1_PIN A0 // датчик давления 1 на пин A0
#define SENSOR2_PIN A1 // датчик давления 2 на пин A1
#define SENSOR3_PIN A2 // датчик давления 3 на пин A2
#define SENSOR4_PIN A3 // датчик давления 4 на пин A3

#define PWMA 11
#define AIN1 7
#define AIN2 6

#define PWMB 10
#define BIN1 5
#define BIN2 4

#define STBY 13

#define SENSOR_LOW 100  // нижний порог значений с датчика (от 0 до 1023)
#define SENSOR_HIGH 400 // верхний порог значений с датчика (от 0 до 1023)

#define MAX_WORKING_TIME 20000 // макисмальное время работы мотора (в миллисекундах)

int sensor1; // переменная, которая будет хранить значения с датчика 1
int sensor2; // переменная, которая будет хранить значения с датчика 2
int sensor3; // переменная, которая будет хранить значения с датчика 3
int sensor4; // переменная, которая будет хранить значения с датчика 4

unsigned int timer_motor1; // таймер работы мотора 1 (в миллисекундах)
unsigned int timer_motor2; // таймер работы мотора 2 (в миллисекундах)

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

  digitalWrite(STBY, HIGH);
}

void loop()
{
  // чтение значений с датчиков и запись значений в переменные
  sensor1 = analogRead(SENSOR1_PIN);
  sensor2 = analogRead(SENSOR2_PIN);
  sensor3 = analogRead(SENSOR3_PIN);
  sensor4 = analogRead(SENSOR4_PIN);

  // вывод значений с датчиков на компьютер
  Serial.print(sensor1);
  Serial.print(" | ");
  Serial.print(sensor2);
  Serial.print(" | ");
  Serial.print(sensor3);
  Serial.print(" | ");
  Serial.println(sensor4);

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

    // обнуляем таймер работы мотора 1
    timer_motor1 = 0;
  }
  else if (
      (sensor1 < SENSOR_LOW and sensor2 < SENSOR_LOW) and
      timer_motor1 < MAX_WORKING_TIME)
  {
    // Если оба значения с датчиков 1 и 2 меньше значения В,
    // то включаем мотор-редуктор1 против часовой стрелки, чтобы замотать
    // трос и увеличить давление до нужного.

    // включаем мотор 1 против часовой стрелки
    motor1_backward();

    // обновляем значение переменной таймера, для слежения за временем работы мотора
    timer_motor1 = millis();
  }
  else if (
      (sensor1 > SENSOR_HIGH or sensor2 > SENSOR_HIGH) and
      timer_motor1 < MAX_WORKING_TIME)
  {
    // Если хотя бы один из значений датчиков 1 или 2 больше значения В,
    // то включаем мотор-редуктор1 по часовой стрелки, чтобы размотать
    // трос и уменьшить давление до нужного.

    // включаем мотор 2 по часовой стрелке
    motor1_forward();

    // обновляем значение переменной таймера, для слежения за временем работы мотора
    timer_motor1 = millis();
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

    // обнуляем таймер работы мотора 2
    timer_motor2 = 0;
  }
  else if (
      (sensor3 < SENSOR_LOW and sensor4 < SENSOR_LOW) and
      timer_motor2 < MAX_WORKING_TIME)
  {
    // Если оба значения с датчиков 3 и 4 меньше значения В,
    // то включаем мотор-редуктор2 против часовой стрелки, чтобы замотать
    // трос и увеличить давление до нужного.

    // включаем мотор 2 против часовой стрелки
    motor2_backward();

    // обновляем значение переменной таймера, для слежения за временем работы мотора
    timer_motor2 = millis();
  }
  else if (
      (sensor3 > SENSOR_HIGH or sensor4 > SENSOR_HIGH) and
      timer_motor2 < MAX_WORKING_TIME)
  {
    // Если хотя бы один из значений датчиков 3 или 4 больше значения В,
    // то включаем мотор-редуктор2 по часовой стрелки, чтобы размотать
    // трос и уменьшить давление до нужного.

    // включаем мотор 2 по часовой стрелке
    motor2_forward();

    // обновляем значение переменной таймера, для слежения за временем работы мотора
    timer_motor2 = millis();
  }
}
