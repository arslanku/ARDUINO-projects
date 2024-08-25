#include <Arduino.h>

/*
   Простой мультимедиа пульт, всего 1 энкодер для управления:
    -Поворот влево              volume down
    -Поворот вправо             volume up
    -Поворот влево + нажатие    previous track
    -Поворот влево + нажатие    next track
    -Клик по энкодеру           play / pause
*/

#define _EB_HOLD 300  // Переопределяем таймаут удержания
#define ENC_CLICK 400 // это про задержку Ягмуров
#define ENC_CK 7      // CK пин энкодера
#define ENC_DT 6      // DT пин энкодера
#define ENC_SW 8      // SW пин энкодера (кнопка)

#include <EncButton.h> // Либа энкодера
#include <EasyHID.h>   // Либа HID

EncButton<EB_TICK, ENC_CK, ENC_DT, ENC_SW> enc; // Обьект энкодера
// EncButton enc(ENC_CK, ENC_DT, ENC_SW);

void setup()
{
  HID.begin(); // Запускаем USB
  // enc.setClickTimeout(ENC_CLICK);
  // enc.setHoldTimeout(_EB_HOLD);
}

void loop()
{
  enc.tick();
  HID.tick(); // Вызываем тикер не реже чем каждые 10 мс!

  if (enc.hasClicks(2))
  {
    Keyboard.clickMultimediaKey(KEY_STOP); // Двойное нажатие (Ягмуров)
  }
  else if (enc.click())
  {
    Keyboard.clickMultimediaKey(KEY_PLAYPAUSE); // Клик по кнопке - playpause
  }
  else if (enc.isRight())
  {
    Keyboard.clickMultimediaKey(KEY_VOL_UP); // Вращение вправо - vol+
  }

  else if (enc.isLeft())
  {
    Keyboard.clickMultimediaKey(KEY_VOL_DOWN); // Вращение влево - vol-
  }

  else if (enc.isRightH())
  {
    Keyboard.clickMultimediaKey(KEY_NEXT_TRACK); // Вращение вправо с нажатием - next track
  }

  else if (enc.isLeftH())
  {
    Keyboard.clickMultimediaKey(KEY_PREV_TRACK); // Вращение влево с нажатием - previous track
  }  
}