#include <Arduino.h>

// пин для подключения фоторезистора
#define FOTORESISTOR_PIN A7

// уровень для срабатывания сбросника (минимум - 0, максимум - 1024)
#define TRIGGER_LEVEL 500

// пин для подключения сервопривода
#define SERVO_PIN 9

// задержка после сброса (указывать в миллисекундах)
#define SERVO_DELAY 1500

// угол сервопривода в исходном положении
#define START_SERVO_POSITION 0

// угол сервопривода при срабатывании сбросника
#define END_SERVO_POSITION 90

// связь с компьютером, передача информации: о срабатывании сервопривода, значение с фоторезистора
// 1 - включено, 0 - выключено
#define LOGGING 1

#include <Servo.h>
Servo servo;

void setup()
{
#if (LOGGING == 1)
  Serial.begin(9600);
#endif

  servo.attach(SERVO_PIN);
  servo.write(START_SERVO_POSITION);
}

bool ready = true;
int fotoresistor_value;
void loop()
{
  fotoresistor_value = analogRead(FOTORESISTOR_PIN);

  if ((fotoresistor_value > TRIGGER_LEVEL) && ready)
  {
#if (LOGGING == 1)
    Serial.print("Сервопривод сработал. Уровень света: ");
    Serial.println(fotoresistor_value);
#endif

    servo.write(END_SERVO_POSITION);
    delay(SERVO_DELAY);
    servo.write(START_SERVO_POSITION);
    ready = false;
  }
  else if ((fotoresistor_value < TRIGGER_LEVEL) && !ready)
  {
#if (LOGGING == 1)
    Serial.print("Уровень света: ");
    Serial.print(fotoresistor_value);
    Serial.println(". Сервопривод готов к работе");
#endif
    ready = true;
  }
}
