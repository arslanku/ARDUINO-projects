void buttons()
{
  if (menu == 0) // MAIN MENU
  {
    if (btn1.press()) // дым
    {
      relayState[2] = digitalRead(relayPins[2]);
      digitalWrite(RELAY3_PIN, !relayState[2]);

      lcd.setCursor(0, 0);
      lcd.print(" ДЫМ-");
      lcd.print(digitalRead(RELAY3_PIN), DEC);
    }
    if (btn2.press()) // конвекция
    {
      relayState[3] = digitalRead(relayPins[3]);
      digitalWrite(RELAY4_PIN, !relayState[3]);

      lcd.setCursor(11, 0);
      lcd.print("КОНВ-");
      lcd.print(digitalRead(RELAY4_PIN), DEC);
    }
    if (btn3.press()) // сушка
    {
      bool state = digitalRead(RELAY5_PIN);
      digitalWrite(RELAY5_PIN, !state);

      lcd.setCursor(0, 1);
      lcd.print("СУШК-");
      lcd.print(digitalRead(RELAY5_PIN), DEC);
    }
    if (btn4.press()) // свет
    {
      bool state = digitalRead(RELAY6_PIN);
      digitalWrite(RELAY6_PIN, !state);

      lcd.setCursor(11, 1);
      lcd.print("СВЕТ-");
      lcd.print(digitalRead(RELAY6_PIN), DEC);
    }
  }

  else if (menu == 1) // SETTING 1
  {
    if (btn1.press() || btn1.step())
    {
      data.temp1_min += 1;
      mem.update();
      display();
    }
    else if (btn2.press() || btn2.step())
    {
      data.temp1_min -= 1;
      mem.update();
      display();
    }
  }

  else if (menu == 2) // SETTING 1
  {
    if (btn1.press() || btn1.step())
    {
      data.temp1_max += 5;
      mem.update();
      display();
    }
    else if (btn2.press() || btn2.step())
    {
      data.temp1_max -= 5;
      mem.update();
      display();
    }
  }

  else if (menu == 3) // SETTING 2
  {
    if (btn1.press() || btn1.step())
    {
      data.temp2 += 5;
      mem.update();
      display();
    }
    else if (btn2.press() || btn2.step())
    {
      data.temp2 -= 5;
      mem.update();
      display();
    }
  }

  if (btn5.press()) // NEXT MENU
  {
    if (menu < 3)
    {
      menu++;
    }
    else
    {
      menu = 0;
    }
    display();
  }
}
