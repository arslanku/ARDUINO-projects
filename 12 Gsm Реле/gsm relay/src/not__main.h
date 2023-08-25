// #include <Arduino.h>

// String num1 = "0123456789";
// String num2 = "9876543210";

// #include <EEPROM.h>

// // void set_new_number(String new_number_str)
// // {
// //   Serial.println("set_new_number");
// //   for (int i = 0; i < EEPROM.length(); i++)
// //   {
// //     if (EEPROM[i] == 255)
// //     {
// //       uint64_t number_long = new_number_str.toInt();
// //       EEPROM.put(i, number_long);
// //       Serial.println("set_new_number: OK");
// //       break;
// //     }
// //   }
// // }
// // void check_number(String number)
// // {
// //   for (int i = 0; i < EEPROM.length(); i++)
// //   {
// //     if (EEPROM[i] == number)
// //     {
// //       return;
// //       break;
// //     }
// //   }
// // }
// void get_all_data()
// {
//   Serial.println("get_all_data");
//   for (int i = 0; i < EEPROM.length(); i++)
//     Serial.println(EEPROM[i]);
// }
// void reset_all_data()
// {
//   for (int i = 0; i < EEPROM.length(); i++)
//     EEPROM[i] = 255;
// }

// // String data1 = "";
// // String data2 = "";
// // String data3 = "";
// // String data4 = "";
// // String data5 = "";
// // #include <EEManager.h>
// // EEManager memory(data1);
// // EEManager memory(data2);
// // EEManager memory(data3);
// // EEManager memory(data4);
// // EEManager memory(data5);

// // void set_new_number(String new_number_str)
// // {
// //   for (byte i = 0; i <= 5; i++)
// //   {
    
// //   }
// // }

// // void set_new_number(String new_number)
// // {
// //   Serial.println("set_new_number");
// //   for (int i = 0; i < 1024; i++)
// //   {
// //     memory.begin(i, 0);
// //     // Serial.println(data);
// //     if (data == "\0")
// //     {
// //       data = new_number;
// //       memory.update();
// //       Serial.println(data);
// //       Serial.println("Break \n");
// //       break;
// //     }
// //   }
// // }
// // bool check_number(String number)
// // {
// //   Serial.println("check_number");
// //   for (int i = 0; i < 1024; i++)
// //   {
// //     memory.begin(i, 0);
// //     // Serial.println(data);
// //     if (data == number)
// //     {
// //       Serial.print("Найден номер: ");
// //       Serial.println(data);
// //       Serial.println("Break \n");
// //       break;
// //     }
// //   }
// // }
// // void get_all_data()
// // {
// //   Serial.println("get_all_data");
// //   for (int i = 0; i < 1024; i++)
// //   {
// //     memory.begin(i, 0);
// //     Serial.println(data);
// //   }
// //   Serial.println("Stop");
// // }

// void setup()
// {
//   Serial.begin(9600);

//   // byte request = memory.begin(0, 0);
//   // Serial.println(request);

//   // for (int i = memory.startAddr(); i < memory.endAddr() + 1; i++)
//   //   Serial.write(EEPROM.read(i));
//   // Serial.println();

//   // set_new_number(num1);
//   // set_new_number(num2);

//   // check_number("0123456789");
//   reset_all_data();
//   get_all_data();
// }

// void loop()
// {
//   // memory.tick();
// }
