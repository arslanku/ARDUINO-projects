void parseSMS(String msg)
{
  String msgheader = "";
  String msgbody = "";
  String msgphone = "";

  msg = msg.substring(msg.indexOf("+CMGR: "));
  msgheader = msg.substring(0, msg.indexOf("\r")); // Выдергиваем телефон

  msgbody = msg.substring(msgheader.length() + 2);
  msgbody = msgbody.substring(0, msgbody.lastIndexOf("OK")); // Выдергиваем текст SMS
  msgbody.trim();

  int firstIndex = msgheader.indexOf("\",\"") + 3;
  int secondIndex = msgheader.indexOf("\",\"", firstIndex);
  msgphone = msgheader.substring(firstIndex, secondIndex);

  Serial.println("Phone: " + msgphone);  // Выводим номер телефона
  Serial.println("Message: " + msgbody); // Выводим текст SMS

  if (msgphone.length() > 6 && phones.indexOf(msgphone) > -1)
  { // Если белом списке
    Serial.println(msgbody);
    Serial.println(msgphone);

    if (msgbody == "1")
      digitalWrite(13, 1);
    else if (msgbody == "0")
      digitalWrite(13, 0);
  }
  else
    Serial.println("Unknown phonenumber");
}

void sendSMS(String phone, String message)
{
  sendATCommand("AT+CMGS=\"" + phone + "\"", true);           // Переходим в режим ввода текстового сообщения
  sendATCommand(message + "\r\n" + (String)((char)26), true); // После текста отправляем перенос строки и Ctrl+Z
}

// void setLedState(String result, String phone)
// {
//   bool correct = false; // Для оптимизации кода, переменная корректности команды
//   String msgToSend = "";
//   if (result.length() == 2)
//   {
//     int ledIndex = ((String)result[0]).toInt(); // Получаем первую цифру команды - адрес устройства (1-3)
//     int ledState = ((String)result[1]).toInt(); // Получаем вторую цифру команды - состояние (0 - выкл, 1 - вкл)
//     if (ledIndex >= 1 && ledIndex <= 3 && (ledState == 0 or ledState == 1))
//     {                                                                                      // Если все нормально, исполняем команду
//       msgToSend = "LED:" + (String)ledIndex + " set to " + (ledState == 0 ? "OFF" : "ON"); // Статус исполнения
//       digitalWrite(pins[ledIndex - 1], ledState);                                          // Исполняем команду
//       correct = true;                                                                      // Флаг корректности команды
//     }
//   }
//   if (!correct)
//   {
//     msgToSend = "Incorrect command: " + result; // Статус исполнения
//   }
//   Serial.println(msgToSend); // Выводим результат в терминал
// }
