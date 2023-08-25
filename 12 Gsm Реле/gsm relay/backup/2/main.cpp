// =================== НАСТРОЙКИ ===================

#define PASSWORD_SET "1010"
#define PASSWORD_DEL "2020"

#define CODE_ON "1"
#define CODE_OFF "0"

#define SERIAL_SPEED 9600  // скорость сериал порта
#define SIM800L_SPEED 9600 // скорость сериал для модуля

#define RX_PIN 8 // пин для подключения (TX SIM800L -> RX Arduino)
#define TX_PIN 9 // пин для подключения (RX Arduino -> TX SIM800L)

#define RELAY_PIN_1 4
#define RELAY_PIN_2 6

// =================================================

#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial SIM800(RX_PIN, TX_PIN);

String _response = "";
void setup()
{
  Serial.begin(9600);
  SIM800.begin(9600);

  Serial.println("Начало работы...");

  pinMode(RELAY_PIN_1, 1);
  pinMode(RELAY_PIN_2, 1);

  _response = sendATCommand("AT", 1);            // Проверка общего статуса
  _response = sendATCommand("ATE0V1+CMEE=2", 1); // Echo Mode (ATE0), текстовый формат ответов модуля (ATV1) и текстовый код ошибок (AT+CMEE=2)
  _response = sendATCommand("AT+DDET=1,0,0", 1); // Включаем DTMF
}

String sendATCommand(String cmd, bool waiting)
{
  String _resp = "";
  Serial.println(cmd);
  SIM800.println(cmd);
  
  if (waiting)
  {
    _resp = waitResponse();
    if (_resp.startsWith(cmd))
    {
      _resp = _resp.substring(_resp.indexOf("\r\n", cmd.length()) + 2);
    }

    Serial.println(_resp);
  }
  return _resp;
}

String waitResponse()
{
  String _resp = "";
  long _timeout = millis() + 10000;
  while (!SIM800.available() && millis() < _timeout)
  {
  };

  if (SIM800.available())
  {
    _resp = SIM800.readString();
  }
  else
  {
    Serial.println("Ошибка таймаута");
  }

  return _resp;
}

void loop()
{
  if (SIM800.available())
  {
    _response = waitResponse();
    Serial.println(">" + _response);
    int index = -1;

    do
    {
      index = _response.indexOf("\r\n");
      String submsg = "";

      if (index > -1)
      {
        submsg = _response.substring(0, index);
        _response = _response.substring(index + 2);
      }
      else
      {
        submsg = _response;
        _response = "";
      }

      submsg.trim();

      if (submsg != "")
      {
        Serial.println("submessage: " + submsg);

        if (submsg.startsWith("+DTMF:"))
        {
          String symbol = submsg.substring(7, 8);

          int phoneindex = _response.indexOf("+CLIP: \"");
          String innerPhone = "";
          if (phoneindex >= 0)
          {
            phoneindex += 8;
            innerPhone = _response.substring(phoneindex, _response.indexOf("\"", phoneindex));
          }

          processingDTMF(symbol, innerPhone);
        }
        else if (submsg.startsWith("RING"))
        {
          int phoneindex = _response.indexOf("+CLIP: \"");
          String innerPhone = "";
          if (phoneindex >= 0)
          {
            phoneindex += 8;
            innerPhone = _response.substring(phoneindex, _response.indexOf("\"", phoneindex));
            Serial.println("[INFO] Входящий вызов!");
            Serial.println("Номер: " + innerPhone);
          }

          sendATCommand("ATA", true);
        }
      }
    } while (index > -1);
  }

  if (Serial.available())
  {
    SIM800.write(Serial.read());
  }
}

String result = "";

void processingDTMF(String symbol, String number)
{
  Serial.println("Ключ: " + symbol);

  if (symbol == "#")
  {
    if (result == PASSWORD_SET)
    {
      Serial.println("[INFO] Назначение номера в EEPROM: " + number);
      
    }
    else if (result == PASSWORD_DEL)
    {
      Serial.println("[INFO] Удаление номера в EEPROM: " + number);
    }
    else if (result == CODE_ON)
    {
      Serial.println("[INFO] Включение реле 1");
      digitalWrite(RELAY_PIN_1, 1);

      int relay_timer = millis();
      if (millis() - relay_timer >= 5000)
      {
        Serial.println("[INFO] Включение реле 2");
        digitalWrite(RELAY_PIN_2, 1);
      }
    }
    else if (result == CODE_OFF)
    {
      Serial.println("[INFO] Выключение реле 1 и 2");
    }
    result = "";
  }
  else
  {
    result += symbol;
  }
}
