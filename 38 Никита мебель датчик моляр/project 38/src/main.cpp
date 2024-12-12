#include <Arduino.h>

#define BTN_PIN 5

#define _LCD_TYPE 1
#include <LCD_1602_RUS_ALL.h>
LCD_1602_RUS lcd(0x27, 16, 2);

void setup()
{
  // Serial.begin(9600);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("часы : мин : сек");
  lcd.setCursor(0, 1);
  lcd.print("     :     :    ");

  pinMode(BTN_PIN, INPUT_PULLUP);
}

uint32_t noWorkingSec, workingSec, dispTimer;
void loop()
{
  if (digitalRead(BTN_PIN) == 0)
  {
    noWorkingSec = millis() - workingSec;
  }
  else if (digitalRead(BTN_PIN) == 1)
  {
    workingSec = millis() - noWorkingSec;
  }

  uint32_t sec = workingSec / 1000ul;   // полное количество секунд
  int timeHours = (sec / 3600ul);       // часы
  int timeMins = (sec % 3600ul) / 60ul; // минуты
  int timeSecs = (sec % 3600ul) % 60ul; // секунды

  if (millis() - dispTimer >= 1000)
  {
    dispTimer = millis();
    lcd.setCursor(1, 1);
    lcd.print(String(timeHours) + " ");
    lcd.setCursor(7, 1);
    lcd.print(String(timeMins) + " ");
    lcd.setCursor(13, 1);
    lcd.print(String(timeSecs) + " ");
  }
  
  // static uint32_t timer;
  // if (millis() - timer >= 1000)
  // {
  //   timer = millis();
  //   Serial.print(millis());
  //   Serial.print(" | ");
  //   Serial.print(sec);
  //   Serial.print(" | ");
  //   Serial.print(noWorkingSec);
  //   Serial.print(" | ");
  //   Serial.println(digitalRead(BTN_PIN));
  // }
}
