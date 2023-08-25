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

  Serial.println("Телефон: " + msgphone);  // Выводим номер телефона
  Serial.println("Сообщение: " + msgbody); // Выводим текст SMS

  if (msgphone.length() > 6)
  {
    Serial.println(msgbody);
    Serial.println(msgphone);

    // if (msgbody == PASSWORD_FOR_SET) // запись в EEPROM
    // {
      
    // }
    // if (msgbody == PASSWORD_FOR_DEL) // удаление из EEPROM
    // {

    // }
  }
  else
    Serial.println("Unknown phonenumber");
}

void sendSMS(String phone, String message)
{
  sendATCommand("AT+CMGS=\"" + phone + "\"", true);           // Переходим в режим ввода текстового сообщения
  sendATCommand(message + "\r\n" + (String)((char)26), true); // После текста отправляем перенос строки и Ctrl+Z
}
