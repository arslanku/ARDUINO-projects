// ***************** nsk-electro.ru ***** Семерков Николай **** 2018
// Пример кода осуществляющий чтение температуры датчиков DS18B20, измерение напряжения АКБ
// и передачу для визуализации и логгирования на сервис open-monitoring.online

// Строки требующие редактирования в комментариях отмечены (!)

//*-- Подключение библиотек
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "TA12-100.h"

long nextTime = 100000; // Служебная переменная

//*-- Настройки контроллера
#define SSID "RT-WiFi-A640"             // Название точки доступа     (!)
#define PASS "evYbyuMAEi"               // Пароль                     (!)
String IP = "open-monitoring.online";   // Адрес сервера (не менять)

#define esp_bitrate 9600  // Скорость передачи данных между arduino и ESP8266
#define rxpin 10          //RX
#define txpin 11          //TX


#define one_wire_pin 2          // Вход для подключения датчиков тем-ры
//#define AnalogIn1 A0          // Аналоговый вход 1  (Напряжение АКБ)

//*-- Параметры полученные при регистрации на http://open-monitoring.online
String ID = "2209"; // ID                                             (!)
String KEY = "hIq1IM"; // Код доступа                                (!)
long interval = 120000; // Периодичность отправки пакетов на сервер (120 секунд)

//*-- Работа с несколькими датчиками по одной шине
OneWire oneWire(one_wire_pin);
DallasTemperature sensors(&oneWire);

//*-- Software Serial
SoftwareSerial espSerial( rxpin, txpin ); // RX, TX

//*-- GET Information
String GET = "GET /get?cid=" + ID + "&key=" + KEY; // GET request
String HOST = "Host: " + IP + "\r\n\r\n";



void setup() {

  Serial.begin(esp_bitrate);
  espSerial.begin(esp_bitrate);
  sensors.begin();             //Инициализация опроса датчиков температуры DS18B20 (если используются)

  sendespSerial("AT");
  delay(2000);
  if (espSerial.find("OK"))
  {
    Serial.println("RECEIVED ESP: OK\nData ready to sent!");
    connectWiFi();
  }
}


void loop() {
  //*-- Измерение температуры
  sensors.requestTemperatures();
  int t1 = round(sensors.getTempCByIndex(0));
  delay(2000);
  int t2 = round(sensors.getTempCByIndex(1));
  delay(2000);
  int t3 = round(sensors.getTempCByIndex(2));
  delay(2000);
  int t4 = round(sensors.getTempCByIndex(3));
  delay(2000);
  int t5 = round(sensors.getTempCByIndex(4));
  delay(2000);

  String T1 = String(t1); // turn integer to string
  String T2 = String(t2); // turn integer to string
  String T3 = String(t3); // turn integer to string
  String T4 = String(t4); // turn integer to string
  String T5 = String(t5); // turn integer to string


  //*-- Измерение напряжения делителем (делается 100 замеров и усредняется)
  //  float Volt = 0;
  //  for (int i = 0; i < 100; i++) {
  //    Volt = Volt + (0.0304 * analogRead(AnalogIn1)) / 100; // Пропорция 25,22/3,9В
  //    delay(1);
  //  }
  //  String Uakb = String(Volt);


  Serial.println(millis());
  Serial.print("Температура датчика 1: ");
  Serial.println(T1);
  Serial.print("Температура датчика 2: ");
  Serial.println(T2);
  Serial.print("Температура датчика 3: ");
  Serial.println(T3);
  Serial.print("Температура датчика 4: ");
  Serial.println(T4);
  Serial.print("Температура датчика 5: ");
  Serial.println(T5);
  if ( millis() - nextTime > (interval - 4215) ) {
    update_param(T1, T2, T3, T4, T5); // Отправляем пакет на сервер, если добавляются еще параметры - дописать через запятую
    nextTime = millis();
  }
}


//*-- Функции --*//
//*--Функция отправки в порт ESP и терминал
void sendespSerial(String cmd) {
  Serial.print("SEND: ");
  Serial.println(cmd);
  espSerial.println(cmd);
}


//*-- Соединение с точкой доступа
boolean connectWiFi() {
  espSerial.println("AT+CWMODE=1"); // Режим работы "клиент"
  delay(2000);
  String cmd = "AT+CWJAP=\"";       // Подключение к точке доступа
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";
  sendespSerial(cmd);
  delay(5000);
  if (espSerial.find("OK"))
  {
    Serial.println("Connect: OK");
    return true;
  }
  else
  {
    Serial.println("Connect: Error");
    return false;
  }

  cmd = "AT+CIPMUX=0";           // Режим одиночного соединения
  sendespSerial( cmd );
  if ( espSerial.find( "Error") )
  {
    Serial.print( "RECEIVED: Error" );
    return false;
  }
}


//*--- Отправка данных на сервер, если есть параметры, так же дописать через запятую с указанием типа, например: String T3
void update_param( String T1, String T2, String T3, String T4, String T5 ) {

  String cmd = "AT+CIPSTART=\"TCP\",\"";// Setup TCP connection
  cmd += IP;
  cmd += "\",80";
  sendespSerial(cmd);
  delay(1000);

  if ( espSerial.find( "Error" ) )
  {
    Serial.print( "RECEIVED: Error\nExit1" );
    return;
  }

  // Отправка строки на сервер
  cmd = GET + "&p1=" + T1 + "&p2=" + T2 + "&p3=" + T3 ; //
  cmd += "&p4=" + T4 + "&p5=" + T5 ;                 // Раскомментировать для 3-го датчика
  cmd += " HTTP/1.1\r\n"  + HOST;
  espSerial.print( "AT+CIPSEND=" );
  espSerial.println( cmd.length() );
  if (espSerial.find( ">" ) )
  {
    Serial.print(">");
    Serial.print(cmd);
    espSerial.print(cmd);
    delay(30);
  }
  else
  {
    sendespSerial( "AT+CIPCLOSE" );//close TCP connection
  }
  if ( espSerial.find("OK") )
  {
    Serial.println( "RECEIVED: OK" );
  }
  else
  {
    Serial.println( "RECEIVED: Error\nExit2" );
  }
  serialread(); // Закомментировать после отладки
}


//*-- Отладочная функция (закомментировать после наладки)
String s = "";
void serialread() {
  while (espSerial.available()) {
    delay(20);
    if (espSerial.available() > 0) {
      char c = espSerial.read();
      s += c;
    }
  }
  Serial.println(s);
  Serial.println("\r\n");
  s = "";
}
