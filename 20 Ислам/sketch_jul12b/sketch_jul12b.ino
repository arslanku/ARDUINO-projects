uint32_t tm1, tm2;

void setup() {
  pinMode(13, 1);
  pinMode(7, 1);

  pinMode(12, 1);
  pinMode(8, 1);

  digitalWrite(13, 1);
  digitalWrite(7, 1);
  tm1 = millis();
}

void loop() {
  static bool flag = 1;

  if (digitalRead(13) && millis() - tm1 >= 300) {
    tm1 = millis();
    digitalWrite(13, 0);
    digitalWrite(7, 0);
  }

  if (!digitalRead(13) && millis() - tm1 >= 5) {
    tm1 = millis();
    digitalWrite(13, 1);
    digitalWrite(7, 1);
  }


  if (millis() >= 150) {
    if (flag) {
      tm2 = millis();
      flag = 0;
      digitalWrite(12, 1);
      digitalWrite(8, 1);
    }

    if (digitalRead(12) && millis() - tm2 >= 300) {
      tm2 = millis();
      digitalWrite(12, 0);
      digitalWrite(8, 0);
    }

    if (!digitalRead(12) && millis() - tm2 >= 5) {
      tm2 = millis();
      digitalWrite(12, 1);
      digitalWrite(8, 1);
    }
  }
}
