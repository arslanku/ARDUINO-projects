// размеры клавиатуры
#define KP_ROWS 4
#define KP_COLS 4

// пины подключения (по порядку штекера)
byte colPins[KP_COLS] = {7, 6, 5, 4};
byte rowPins[KP_ROWS] = {11, 10, 9, 8};

// массив имён кнопок
char keys[KP_ROWS][KP_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

#include <SimpleKeypad.h>

SimpleKeypad keypad((char *)keys, rowPins, colPins, KP_ROWS, KP_COLS);

int keyboard()
{
  char key = keypad.getKey();

  if (key)
  {
    Serial.println(key);

    if (key == 'A') // начать набор номера
    {
      Serial.println("Начните набор");

      disp.clear();
      disp.displayByte(0x08, 0x08, 0x08, 0x08);
      disp_timer = millis();

      long keyboard_timer = millis();

      String data_str = "";
      char key2;

      while (1)
      {
        key2 = keypad.getKey(); // обработка нажатия
        if (key2 != '\0')       // если была нажата
        {
          if (key2 == 'B') // если нажата B
          {
            Serial.print("Данные: ");
            Serial.println(data_str);

            int data_int = data_str.toInt();
            return data_int;

            break;
          }

          else if (key2 == 'D') // если нажата C
          {
            Serial.println("Ввод прекращен");

            data_str = ""; // начать ввод сначала

            break;
          }

          else if (keyboard_timer - millis() >= 5000)
          {
            disp.clear();
            disp.displayByte(0x08, 0x08, 0x08, 0x08);
            disp_timer = millis();
          }

          else
          {
            data_str += key2; // прибавить нажатую цифру к номеру
            Serial.println(data_str);
            keyboard_timer = millis();

            int data_int = data_str.toInt();
            disp.clear();
            disp.displayInt(data_int);
            disp_timer = millis();
          }
        }
      }
    }
  }
}
