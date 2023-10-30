void display()
{
  if (menu == 0) // MAIN MENU
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" ДЫМ-");
    lcd.print(digitalRead(RELAY3_PIN), DEC);

    lcd.setCursor(11, 0);
    lcd.print("КОНВ-");
    lcd.print(digitalRead(RELAY4_PIN), DEC);

    lcd.setCursor(0, 1);
    lcd.print("СУШК-");
    lcd.print(digitalRead(RELAY5_PIN), DEC);

    lcd.setCursor(11, 1);
    lcd.print("СВЕТ-");
    lcd.print(digitalRead(RELAY6_PIN), DEC);

    lcd.setCursor(0, 2);
    lcd.print("ТЕМП1:");
    lcd.print(temp[0], DEC);
    lcd.print("С");

    lcd.setCursor(11, 2);
    lcd.print("ТЕМП2:");
    lcd.print(temp[1], DEC);
    lcd.print("С");

    lcd.setCursor(0, 3);
    lcd.print("ТЕМП3:");
    lcd.print(temp[2], DEC);
    lcd.print("С");
  }

  else if (menu == 1) // SETTING 1
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("НАСТРОЙКА ТЭН 1 2");
    lcd.setCursor(0, 2);
    lcd.print("ТЕМП ВКЛ: ");
    lcd.print(data.temp1_min, DEC);
    lcd.print("С");
  }

  else if (menu == 2) // SETTING 2
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("НАСТРОЙКА ТЭН 1 2");
    lcd.setCursor(0, 2);
    lcd.print("ТЕМП ВЫКЛ: ");
    lcd.print(data.temp1_max, DEC);
    lcd.print("С");
  }

  else if (menu == 3) // SETTING 3
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("НАСТРОЙКА ТЭН 3");
    lcd.setCursor(0, 2);
    lcd.print("ТЕМП ВЫКЛ: ");
    lcd.print(data.temp2, DEC);
    lcd.print("С");
  }
}