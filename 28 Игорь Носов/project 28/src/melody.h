#include <Arduino.h>

#define buzzerPin 2
#define G_Led 13

int win_melody[] = {100, 300, 500, 800, 600, 800};
int lose_melody[] = {800, 600, 800, 500, 300, 100};

void setup()
{
    pinMode(3, 1);
    digitalWrite(3, 1);
    pinMode(G_Led, 1);
    Serial.begin(9600);
}

bool winFlag = 1;
bool loseFlag = 1;

void loop()
{
    Serial.println(sizeof(win_melody));
    if (winFlag)
    {
        for (byte i = 0; i < sizeof(win_melody) / sizeof(win_melody[0]); i++)
        {
            digitalWrite(G_Led, !digitalRead(G_Led));
            tone(buzzerPin, win_melody[i], 150);
            delay(200);
            // digitalWrite(buzzerPin, 1);
        }
        winFlag = 0;
    }

    delay(2000);

    if (loseFlag)
    {
        for (byte i = 0; i < sizeof(lose_melody) / sizeof(lose_melody[0]); i++)
        {
            digitalWrite(G_Led, !digitalRead(G_Led));
            tone(buzzerPin, lose_melody[i], 150);
            delay(200);
        }
        loseFlag = 0;
    }
}