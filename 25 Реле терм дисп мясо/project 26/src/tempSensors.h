void updTemp()
{
  static uint32_t tmr;
  if (millis() - tmr >= 1000)
  {
    tmr = millis();
    for (int i = 0; i < DS_SENSOR_AMOUNT; i++)
    {
      temp[i] = sensor[i].getTemp();
      sensor[i].requestTemp();
    }

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
}