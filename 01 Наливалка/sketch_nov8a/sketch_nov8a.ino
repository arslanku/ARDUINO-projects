#include "config.h"
#include "loader.h"
#include "build.h"
#include "actions.h"

void CONNECT_PORTAL()
{
    portal.attachBuild(build);
    portal.attach(action);
    portal.start();
    Serial.println("Портал запущен!");
    Serial.println("=============================");
    Serial.println("");
}

void CONNECT_WIFI()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("");
    Serial.println("WIFI запущен!");
}

void setup()
{
    Serial.begin(115200);

    CONNECT_WIFI();
    CONNECT_PORTAL();

    pinMode(LED_PIN, OUTPUT);
}

void loop()
{
    portal.tick();
    // delayMicroseconds(5);
    delay(5);
}