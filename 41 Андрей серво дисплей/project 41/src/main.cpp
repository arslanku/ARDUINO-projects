#include <Arduino.h>

#define YGOL_MIN_1 0  // мин угол у первой сервы
#define YGOL_MAX_1 90 // макс угол у первой сервы
#define YGOL_MIN_2 0  // мин угол у второй сервы
#define YGOL_MAX_2 90 // макс угол у второй сервы
#define time_set 5000 // время в мс(1000мс = 1сек) для выхода из редима настройки

#define RELE 13   // D13 пин РЕЛЕ
#define SERVO_1 2 // D2 серво 1
#define SERVO_2 3 // D3 серво 2

#define speed_serv_1 30 // скорость серво 1
#define speed_serv_2 30 // скорость серво 2
#define razgon_s_1 0.1  // разгон сервы 1
#define razgon_s_2 0.1  // разгон сервы 2
#define AMOUNT 2        // кол-во серво

#include <GyverNTC.h>          // нужно УСТАНОВИТЬ библиотеку
#include <LiquidCrystal_I2C.h> // нужно УСТАНОВИТЬ библиотеку
#include <EncButton.h>         // нужно УСТАНОВИТЬ библиотеку
#include <ServoSmooth.h>       // нужно УСТАНОВИТЬ библиотеку
#include <EEPROM.h>

// Библиотеки находят в архиве со скетчем
// ДЛЯ установки Скетч> Подключить библиотеку> Добавить ZIP архив

ServoSmooth servos[AMOUNT];

LiquidCrystal_I2C lcd(0x27, 16, 2); // адрес дисплея 0x27 или 0x3f

EncButton<EB_TICK, 8> btn_plus;  // D8  прибавлять и листать меню
EncButton<EB_TICK, 7> btn_minus; // D7  убавлять и листать меню
EncButton<EB_TICK, 6> btn_ok;    // D6  выбрать и вторым нажатием сохранить
EncButton<EB_TICK, 5> btn_back;  // D5  уйти чтобы снова начать выбирать

// сопротивление резистора 10к
// 0 - пин / 10000 - сопротевление термистора / 3950 - тепловой коэффициент
GyverNTC therm(0, 10000, 3950);      // термистор на пине А0 для Сервы 1
GyverNTC therm_2(1, 10000, 3950);    // термистор на пине А1 для Сервы 2
GyverNTC therm_rele(2, 10000, 3950); // термистор на пине А2 для РЕЛЕ

int temp[] = {13, 18, 10, 20, 15, 25};
int position_1 = 0;
int position_2 = 0;
uint8_t menu = 0;
uint8_t menu_ = 99;
byte x[] = {1, 1, 7, 7, 13, 13};
byte y[] = {0, 1, 0, 1, 0, 1};

byte adres = 1;

byte one_start;
bool set, pl, update, flag_set, update_2;
int temp_one, temp_two, temp_three;
float filT = 0, filT2 = 0, filT3 = 0;

uint32_t timer;
uint32_t timer_2;
uint32_t servoTimer;

void setup()
{
  servos[0].attach(SERVO_1);
  servos[1].attach(SERVO_2);
  servos[0].setSpeed(speed_serv_1);
  servos[0].setAccel(razgon_s_1);
  servos[1].setSpeed(speed_serv_1);
  servos[1].setAccel(razgon_s_2);
  servos[0].setAutoDetach(false); // false удержание, true выкл удержаание
  servos[1].setAutoDetach(false);

  EEPROM.get(0, one_start);

  if (one_start != 67)
  {
    for (byte i = 0; i < 6; i++)
    {
      EEPROM.put(i * sizeof(temp[0]) + adres, temp[i]);
    }
    one_start = 67;
    EEPROM.put(0, one_start);
  }

  for (byte i = 0; i < 6; i++)
  {
    EEPROM.get(i * sizeof(temp[0]) + adres, temp[i]);
  }

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RELE");
  lcd.setCursor(6, 0);
  lcd.print("SRV1");
  lcd.setCursor(12, 0);
  lcd.print("SRV2");

  pinMode(RELE, OUTPUT);
  digitalWrite(RELE, LOW);
}

void tickAllServos()
{
  if (millis() - servoTimer >= 20)
  {
    servoTimer = millis();
    for (byte i = 0; i < AMOUNT; i++)
    {
      servos[i].tickManual();
    }
  }
}

void loop()
{
  tickAllServos();

  if (btn_plus.tick())
    if (btn_plus.click())
    {
      update = true;
      timer_2 = millis();
      if (!flag_set)
      {
        lcd.clear();
        update_2 = true;
      }
      else
      {
        if (!set)
        {
          menu++;
          if (menu >= 6)
            menu = 0;
          pl = true;
        }
        else
          temp[menu]++;
      }
      flag_set = true;
    }

  if (btn_minus.tick())
  {
    if (btn_minus.click())
    {
      update = true;
      timer_2 = millis();
      if (!flag_set)
      {
        lcd.clear();
        update_2 = true;
      }
      else
      {
        if (!set)
        {
          menu--;
          if (menu >= 5)
            menu = 5;
          // mn = true;
        }
        else
          temp[menu]--;
      }
      flag_set = true;
    }
  }

  if (update)
  {
    lcd.setCursor(x[menu] + 1, y[menu]);
    lcd.print(temp[menu]);
    lcd.print(" ");
    update = false;
  }

  if (btn_ok.tick())
    if (btn_ok.click())
    {
      if (!flag_set)
      {
        lcd.clear();
        update_2 = true;
      }
      else
      {
        set = true;
      }
      flag_set = true;
      timer_2 = millis();
      if (set)
        EEPROM.put(menu * sizeof(temp[0]) + adres, temp[menu]);
    }

  if (btn_back.tick())
    if (btn_back.click())
    {
      if (!flag_set)
      {
        lcd.clear();
        update_2 = true;
      }
      else
        set = false;
      flag_set = true;
      timer_2 = millis();
    }

  if (flag_set)
  {
    if (update_2)
    {
      for (byte i = 0; i < 6; i++)
      {
        lcd.setCursor(x[i] + 1, y[i]);
        lcd.print(int(temp[i]));
      }
      lcd.setCursor(0, 0);
      lcd.print("R");
      lcd.setCursor(0, 1);
      lcd.print("L");
      lcd.setCursor(6, 0);
      lcd.print("S");
      lcd.setCursor(6, 1);
      lcd.print("1");
      lcd.setCursor(12, 0);
      lcd.print("S");
      lcd.setCursor(12, 01);
      lcd.print("2");
      update_2 = false;
    }
    
    if (menu_ != menu)
    {
      byte cle;
      if (pl)
      {
        cle = menu - 1;
        if (cle > 5)
          cle = 5;
      }
      else
      {
        cle = menu + 1;
        if (cle > 5)
          cle = 0;
      }
      lcd.setCursor(x[cle], y[cle]);
      lcd.print(" ");
      lcd.setCursor(x[menu], y[menu]);
      lcd.print(">");
      menu_ = menu;
      pl = false;
    }
    if (millis() - timer_2 > time_set)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("RELE:");
      lcd.setCursor(6, 0);
      lcd.print("SRV1");
      lcd.setCursor(12, 0);
      lcd.print("SRV2");
      flag_set = false;
      set = false;
      menu_ = 99;
    }
  }

  if (millis() - timer > 200)
  {
    timer = millis();
    filT += (therm.getTemp() - filT) * 0.1;
    filT2 += (therm_2.getTemp() - filT2) * 0.1;
    filT3 += (therm_rele.getTemp() - filT3) * 0.1;

    int filT_ = filT * 10;
    int filT2_ = filT2 * 10;

    if (temp_one != filT)
    {
      lcd.setCursor(6, 1);
      if (!flag_set)
        lcd.print(filT, 1);
      temp_one = filT;
      int position1 = map(filT_, temp[2] * 10, temp[3] * 10, YGOL_MIN_1 * 10, YGOL_MAX_1 * 10);

      position1 = position1 / 10;
      if (position1 > YGOL_MAX_1)
        position1 = YGOL_MAX_1;
      if (position1 < YGOL_MIN_1)
        position1 = YGOL_MIN_1;
      servos[0].setTargetDeg(position1);
    }
    if (temp_two != filT2)
    {
      lcd.setCursor(12, 1);
      if (!flag_set)
        lcd.print(filT2, 1);
      temp_two = filT2;
      int position2 = map(filT2_, temp[4] * 10, temp[5] * 10, YGOL_MAX_2 * 10, YGOL_MAX_2 * 10);
      position2 = position2 / 10;
      if (position2 > YGOL_MAX_2)
        position2 = YGOL_MAX_2;
      if (position2 < YGOL_MIN_2)
        position2 = YGOL_MIN_2;
      servos[1].setTargetDeg(position2);
    }
    if (temp_three != filT3)
    {
      lcd.setCursor(0, 1);
      if (!flag_set)
        lcd.print(filT3, 1);
      temp_three = filT3;
      if (filT3 > temp[0] && filT3 < temp[1])
      {
        if (!digitalRead(RELE))
          digitalWrite(RELE, HIGH);
      }
      else
      {
        if (digitalRead(RELE))
          digitalWrite(RELE, LOW);
      }
    }
  }
}
