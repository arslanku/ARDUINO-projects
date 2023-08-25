#include <Arduino.h>

uint64_t num1 = 79870000001;
uint64_t num2 = 79870000002;

uint64_t data[10];

#include <EEManager.h>
EEManager mem(data);
// EEManager mem2(arr);

bool set_new_number(uint64_t new_number)
{
  Serial.println("set_new_number");
  for (byte i = 0; i <= 10; i++)
  {
    if (data[i] == new_number)
    {
      return false;
      break;
    }
    else
    {
      if (data[i] == "\0")
      {
        data[i] = new_number;
        mem.updateNow();
        Serial.println("set_new_number: OK\n");
        return true;
        break;
      }
    }
  }
}

bool check_number(uint64_t number)
{
  Serial.println("check_number");
  for (byte i = 0; i <= 10; i++)
  {
    if (data[i] == number)
    {
      Serial.println("check_number: OK\n");
      return true;
    }
  }
}

bool del_number(uint64_t del_number)
{
  Serial.println("del_number");
  for (byte i = 0; i <= 10; i++)
  {
    if (data[i] == del_number)
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
  for (byte i = 0; i <= 10; i++)
  {
    Serial.println(data[i]);
  }
  Serial.println("get_all_numbers: OK\n");
}

void reset_all_numbers()
{
  Serial.println("reset_all_numbers");
  for (byte i = 0; i <= 10; i++)
    data[i] = "\0";

  mem.updateNow();
  Serial.println("reset_all_numbers: OK\n");
}

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(500);

  mem.begin(0, 'a');

  // первый блок храним с адреса 0
  // mem1.begin(0, 'a');

  // второй - следом за ним
  // mem2.begin(mem1.nextAddr, 'a');

  // check_number(num1);
  // del_number(num1);

  // set_new_number(num2);

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
  }
}