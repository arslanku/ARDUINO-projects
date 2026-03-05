#include <Arduino.h>

#define BUTTON_PIN A0
#define BUTTON_LED_PIN A2

#define RX_PIN 7
#define TX_PIN 8

#define QR_CODE1 1000000000
#define QR_CODE2 2000000000
#define QR_CODE3 3000000000
#define QR_CODE4 4000000000

#define SERVO1_PIN 5
#define SERVO2_PIN 6
#define SERVO3_PIN 9
#define SERVO4_PIN 10

#define SERVO_DELAY 10000

#define STRIP_CONNECTED 1
#define BRIGHTNESS 100
#define STRIP_PIN 2

#define NUMLEDS 300
#define COLOR_DEBTH 2

#include <Servo.h>
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

#define _LCD_TYPE 1
#include <LCD_1602_RUS_ALL.h>
LCD_1602_RUS lcd(0x27, 20, 4);

#include <SoftwareSerial.h>
SoftwareSerial scannerSerial(RX_PIN, TX_PIN);

#if STRIP_CONNECTED == 1
#include <microLED.h>
microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER> strip;
#endif

void setup()
{
  pinMode(BUTTON_LED_PIN, OUTPUT);
  digitalWrite(BUTTON_LED_PIN, 1);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(9600);
  scannerSerial.begin(9600);

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);
  servo4.attach(SERVO4_PIN);
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
  servo4.write(0);

  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("    ОТСКАНИРУЙТЕ");
  lcd.setCursor(0, 2);
  lcd.print("       QR КОД");

#if STRIP_CONNECTED == 1
  strip.setBrightness(BRIGHTNESS);
  strip.clear();
  strip.show();
  delay(1);
#endif
}

bool lighting = true;
bool canWin = true;
void loop()
{
  static bool buttonFlag = 0;
  bool buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == 1 && buttonFlag == 0)
  {
    buttonFlag = 1;
    canWin = !canWin;
    digitalWrite(BUTTON_LED_PIN, !digitalRead(BUTTON_LED_PIN));
    delay(50);
  }
  if (buttonState == 0 && buttonFlag == 1)
  {
    buttonFlag = 0;
    delay(50);
  }

#if STRIP_CONNECTED == 1
  static uint32_t strip_timer;
  static bool light_now = false;
  if (lighting)
  {
    if (millis() - strip_timer > 500)
    {
      strip_timer = millis();
      if (light_now)
      {
        strip.clear();
      }
      else
      {
        strip.fill(mWhite);
      }
      light_now = !light_now;
      strip.show();
      delay(1);
    }
  }
#endif

  if (scannerSerial.available())
  {
    lighting = false;

    uint32_t data = scannerSerial.parseInt();
    Serial.println(data);
    scannerSerial.end();

    if (data > 0)
    {
      if (canWin && data == QR_CODE1)
      {
#if STRIP_CONNECTED == 1
        strip.fill(mGreen);
        strip.show();
#endif

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("    ВЫ ВЫИГРАЛИ!    ");
        lcd.setCursor(0, 2);
        lcd.print("   ПОЛУЧИТЕ ПРИЗ    ");

        servo1.write(90);
        delay(SERVO_DELAY);
        servo1.write(0);
      }
      else if (canWin && data == QR_CODE2)
      {
#if STRIP_CONNECTED == 1
        strip.fill(mGreen);
        strip.show();
#endif

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("    ВЫ ВЫИГРАЛИ!    ");
        lcd.setCursor(0, 2);
        lcd.print("   ПОЛУЧИТЕ ПРИЗ    ");

        servo2.write(90);
        delay(SERVO_DELAY);
        servo2.write(0);
      }
      else if (canWin && data == QR_CODE3)
      {
#if STRIP_CONNECTED == 1
        strip.fill(mGreen);
        strip.show();
#endif

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("    ВЫ ВЫИГРАЛИ!    ");
        lcd.setCursor(0, 2);
        lcd.print("   ПОЛУЧИТЕ ПРИЗ    ");

        servo3.write(90);
        delay(SERVO_DELAY);
        servo3.write(0);
      }
      else if (canWin && data == QR_CODE4)
      {
#if STRIP_CONNECTED == 1
        strip.fill(mGreen);
        strip.show();
#endif

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("    ВЫ ВЫИГРАЛИ!    ");
        lcd.setCursor(0, 2);
        lcd.print("   ПОЛУЧИТЕ ПРИЗ    ");

        servo4.write(90);
        delay(SERVO_DELAY);
        servo4.write(0);
      }

      else
      {
#if STRIP_CONNECTED == 1
        strip.fill(mRed);
        strip.show();
#endif

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("    К СОЖАЛЕНИЮ     ");
        lcd.setCursor(0, 2);
        lcd.print("    ВЫ ПРОИГРАЛИ    ");

        delay(SERVO_DELAY);
      }

      data = 0;
      lighting = true;
      scannerSerial.begin(9600);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("    ОТСКАНИРУЙТЕ    ");
      lcd.setCursor(0, 2);
      lcd.print("       QR КОД       ");
    }
  }
}
