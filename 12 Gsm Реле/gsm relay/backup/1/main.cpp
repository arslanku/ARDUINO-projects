#include <Arduino.h>
#include <SoftwareSerial.h>
#include <EEManager.h>

String data = "";
EEManager memory(data);

#include "config.h"

SoftwareSerial SIM800(RX_PIN, TX_PIN); // RX, TX
String _response = "";
long lastUpdate = millis(); // Время последнего обновления
long updatePeriod = 10000;  // Проверять каждые 10 сек

void setup()
{
  Serial.begin(SERIAL_SPEED);  // Скорость обмена данными с компьютером
  SIM800.begin(SIM800L_SPEED); // Скорость обмена данными с модемом
  Serial.println("Start!");

  lastUpdate = millis(); // Обнуляем таймер

  pinMode(13, OUTPUT);
}

#include "waitResponse.h"
#include "sendATCommand.h"
#include "sms.h"

void setupSIM800L()
{
  Serial.println("[INFO] Настройка модуля SIM800L...");

  sendATCommand("AT", 1); // Отправили AT для настройки скорости обмена данными
  // sendATCommand("ATE0V0+CMEE=1;&W", 1); // отключенный Echo Mode (ATE0), цифровой формат ответов модуля (ATV0) и цифровой код ошибок (AT+CMEE=1)
  sendATCommand("ATE0V1+CMEE=2;&W", 1); // Echo Mode (ATE0), текстовый формат ответов модуля (ATV1) и текстовый код ошибок (AT+CMEE=2)
  sendATCommand("AT+CMGF=1;&W", 1);     // Включить TextMode для SMS
  sendATCommand("AT+DDET=1;&W", 1);     // Включаем DTMF
  sendATCommand("AT+CLIP=1;&W", 1);     // АОН
  sendATCommand("AT+CSQ", 1);

  Serial.println("[INFO] Настройка модуля завершена");
}

bool hasmsg = false;
void loop()
{
  static bool flag;
  if (millis() > 500 && flag == 0)
  {
    setupSIM800L();
    flag = 1;
  }

  if (Serial.available())
    sendATCommand(Serial.readString(), true);

  if (SIM800.available())
  {
    _response = waitResponse();
    _response.trim();
    Serial.println(_response);

    if (_response.startsWith("RING"))
    {                                                  // Есть входящий вызов
      int phoneindex = _response.indexOf("+CLIP: \""); // Есть ли информация об определении номера, если да, то phoneindex>-1
      String innerPhone = "";                          // Переменная для хранения определенного номера
      if (phoneindex >= 0)
      {                                                                                    // Если информация была найдена
        phoneindex += 8;                                                                   // Парсим строку и ...
        innerPhone = _response.substring(phoneindex, _response.indexOf("\"", phoneindex)); // ...получаем номер
        Serial.println("[INFO] Входящий вызов!");
        Serial.println("Номер: " + innerPhone); // Выводим номер в монитор порта
      }
    }

    else if (_response.indexOf("+CMTI:") > -1)
      // else if (_response.startsWith("+CMTI:"))
      lastUpdate = millis() - updatePeriod;
  }

  if (lastUpdate + updatePeriod < millis()) // проверить наличие новых сообщений
  {
    do
    {
      _response = sendATCommand("AT+CMGL=\"REC UNREAD\",1", true); // Отправляем запрос чтения непрочитанных сообщений

      if (_response.indexOf("+CMGL: ") > -1)
      { // Если есть хоть одно, получаем его индекс
        int msgIndex = _response.substring(_response.indexOf("+CMGL: ") + 7, _response.indexOf("\"REC UNREAD\"", _response.indexOf("+CMGL: ")) - 1).toInt();
        char i = 0; // Объявляем счетчик попыток
        do
        {
          i++;                                                                   // Увеличиваем счетчик
          _response = sendATCommand("AT+CMGR=" + (String)msgIndex + ",1", true); // Пробуем получить текст SMS по индексу
          _response.trim();                                                      // Убираем пробелы в начале/конце
          if (_response.endsWith("OK"))
          { // Если ответ заканчивается на "ОК"
            if (!hasmsg)
              hasmsg = true;                                    // Ставим флаг наличия сообщений для удаления
            sendATCommand("AT+CMGR=" + (String)msgIndex, true); // Делаем сообщение прочитанным
            sendATCommand("\n", true);                          // Перестраховка - вывод новой строки
            parseSMS(_response);                                // Отправляем текст сообщения на обработку
            break;                                              // Выход из do{}
          }
          else
          {                                 // Если сообщение не заканчивается на OK
            Serial.println("Error answer"); // Какая-то ошибка
            sendATCommand("\n", true);      // Отправляем новую строку и повторяем попытку
          }
        } while (i < 10);
        break;
      }
      else
      {
        lastUpdate = millis(); // Обнуляем таймер
        if (hasmsg)
        {
          sendATCommand("AT+CMGDA=\"DEL READ\"", true); // Удаляем все прочитанные сообщения
          hasmsg = false;
        }
        break;
      }
    } while (1);
  }

  static uint32_t timer;
  if (millis() - timer >= 500)
  {
    digitalWrite(13, !digitalRead(13));
    timer = millis();
  }
}
