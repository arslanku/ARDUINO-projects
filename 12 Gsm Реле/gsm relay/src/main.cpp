#include <Arduino.h>

String data[5];

#include <EEManager.h>
EEManager mem(data);
// EEManager mem2(arr);

String num1 = "0000000001";
String num2 = "0000000002";
String num3 = "0000000003";

bool set_new_number(String new_number_str)
{
  Serial.println("set_new_number");
  for (byte i = 0; i < 5; i++)
  {
    if (data[i] == new_number_str)
    {
      return false;
      break;
    }
    else
    {
      if (data[i] == "\0")
      {
        data[i] = new_number_str;
        mem.updateNow();
        Serial.println("set_new_number: OK\n");
        return true;
        break;
      }
    }
  }
}

bool check_number(String number_str)
{
  Serial.println("check_number");
  for (byte i = 0; i < 5; i++)
  {
    if (data[i] == number_str)
    {
      Serial.println("check_number: OK\n");
      return true;
    }
  }
}

bool del_number(String del_number_str)
{
  Serial.println("del_number");
  for (byte i = 0; i < 5; i++)
  {
    if (data[i] == del_number_str)
    {
      data[i] = "\0";
      mem.updateNow();
      Serial.println("del_number: OK\n");
    }
  }
}

void get_all_numbers()
{
  Serial.println("get_all_numbers");
  for (byte i = 0; i < 5; i++)
  {
    Serial.println(data[i]);
  }
  Serial.println("get_all_numbers: OK\n");
}

void reset_all_numbers()
{
  Serial.println("reset_all_numbers");
  for (byte i = 0; i < 5; i++)
    data[i] = "\0";

  mem.updateNow();
  Serial.println("reset_all_numbers: OK\n");
}

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(500);

  mem.begin(0, 'f');

  // первый блок храним с адреса 0
  // mem1.begin(0, 'a');

  // второй - следом за ним
  // mem2.begin(mem1.nextAddr, 'a');

  // check_number(num1);
  // del_number(num1);

  // set_new_number(num3);

  // get_all_numbers();
}

void loop()
{
  mem.tick();

  if (Serial.available())
  {
    int val = Serial.parseInt();
    Serial.println(val);

    if (val == 0)
      set_new_number(num1);

    else if (val == 1)
      del_number(num1);

    else if (val == 2)
      set_new_number(num2);

    else if (val == 3)
      del_number(num2);

    else if (val == 4)
      get_all_numbers();

    else if (val == 5)
      check_number(num1);

    else if (val == 6)
      check_number(num2);

    else if (val == 7)
      reset_all_numbers();

    else if (val == 8)
      mem.reset();

    else if (val == 9)
    {
      for (int i = 0; i < 5; i++)
      {
        Serial.println(data[i]);
      }
    }
  }
}