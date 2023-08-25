// ======================= НАСТРОЙКИ =======================


#define SENSOR_PIN 2  // пин подключения датчика

#define SENSOR_VALUE_MIN 0     // минимальное показание датчика
#define SENSOR_VALUE_MAX 1023  // макимальное показание датчика

#define SERVO_PIN 5  // пин для подключения сервопривода

#define SERVO_SPEED 80  // скорость сервопривода (град/сек)
#define SERVO_ACCEL 1   // скорость ускорения/торможения

#define SERVO_ANGLE_MIN 0    // минимальный угол сервопривода
#define SERVO_ANGLE_MAX 180  // максимальный угол сервопривода


// ==========================================================


#include <ServoSmooth.h>
ServoSmooth servo;  // объявляем объект класса ServoSmooth

int RPM, position;
long lastflash;


void setup() {

  Serial.begin(9600);
  Serial.println("Начало работы");

  servo.attach(SERVO_PIN);

  servo.setSpeed(SERVO_SPEED);  // ограничить скорость
  servo.setAccel(SERVO_ACCEL);  // установить ускорение (разгон и торможение)

  servo.setTargetDeg(0);

  attachInterrupt(0, sensor, RISING);  //подключить прерывание на 2 пин при повышении сигнала
}


void loop() {
  servo.tick();

  static byte angle = 0;  // переменная для угла сервопривода

  Serial.print("Значение с датчика: ");
  Serial.println(RPM);

  angle = map(RPM, SENSOR_VALUE_MIN, SENSOR_VALUE_MAX, SERVO_ANGLE_MIN, SERVO_ANGLE_MAX);
  /*
  Преобразуем значение получаемое с датчика в значение угла сервопривода
  с помощью функции map()
  */

  Serial.print("Значение угла: ");
  Serial.println(angle);

  servo.setTargetDeg(angle);  //устанавливаем угол на сервоприводе
}


void sensor() {
  RPM = 60 / ((float)(micros() - lastflash) / 1000000);  //расчет количества оборотов
  lastflash = micros();                                  //запомнить время последнего оборота
}
