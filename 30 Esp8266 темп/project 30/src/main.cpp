// =============== НАСТРОЙКА =======================

#define SENSOR_PIN D2
#define REQUEST_TIMER 1
#define RESTART_TIMER 21600

#define WIFI_SSID "TP-Link"
#define WIFI_PASS "indira777semya"

#define MQTT_SERVER "mqtt.dealgate.ru"
#define MQTT_PORT 1883
#define MQTT_USER "skalper101"
#define MQTT_PASSWORD "987654321"
// #define TOPIC_INFO "info"
#define TOPIC_DATA "/data"

#define OUTPUT_PIN 0

// ================ БИБЛИОТЕКИ =====================

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <PubSubClient.h>
WiFiClient espClient;
PubSubClient client(espClient);

#include <microDS18B20.h>
MicroDS18B20<SENSOR_PIN> sensor;

// =============== ПОДКЛЮЧЕНИЕ WI-FI ===============

void WIFI_connect()
{
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("\n[WIFI] Подключение");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");

    if (millis() >= 60 * 1000)
    {
      ESP.restart();
    }
  }
  Serial.println("[WIFI] Подключено успешно");
  Serial.print("[WIFI] IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("[WIFI] SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("[WIFI] MAC: ");
  Serial.println(WiFi.macAddress());
}

// ===================== MQTT ======================

void MQTT_callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("[MQTT MESSAGE] ");
  Serial.print(topic);
  Serial.print(" : ");

  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // String data_pay;
  // for (unsigned int i = 0; i < length; i++)
  // {
  //   data_pay += String((char)payload[i]);
  // }

  // if (String(topic) == TOPIC)
  // {
  //   Serial.println(data_pay);
  // }
}

void MQTT_connect()
{
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(MQTT_callback);

  while (!client.connected())
  {
    Serial.println("[MQTT] Подключение");
    String client_id = String(WiFi.macAddress());
    // String client_id = "ESP8266-client-" + String(WiFi.macAddress());
    Serial.printf("[MQTT] Клиент %s подключается\n", client_id.c_str());

    if (client.connect(client_id.c_str(), MQTT_USER, MQTT_PASSWORD))
    {
      // client.subscribe(TOPIC_DATA);
      Serial.println("[MQTT] Подключено успешно");
    }
    else
    {
      Serial.print("[MQTT ERROR] Ошибка со статусом: ");
      Serial.println(client.state());
      Serial.println("[ESP] ПЕРЕЗАГРУЗКА ДЛЯ ПОВТОРНОГО ПОДКЛЮЧЕНЕИЯ");
      ESP.restart();
    }
  }
}

// =================================================

String temp;
uint32_t sensor_timer;
uint32_t send_timer;
uint32_t wifi_timer;
uint32_t led_timer;

void get_temp()
{
  if (millis() - sensor_timer >= 1000)
  {
    sensor_timer = millis();
    if (sensor.readTemp())
    {
      temp = String(sensor.getTemp());
    }
    else
    {
      temp = "temp sensor error";
    }
    sensor.requestTemp();
  }
}

void send_data()
{
  if (millis() - send_timer >= REQUEST_TIMER * 1000)
  {
    send_timer = millis();

    String data = String(WiFi.macAddress()) + " - " + temp;
    client.publish(TOPIC_DATA, data.c_str());

    Serial.print("[SEND] ");
    Serial.println(data);
  }
}

void check_wifi()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    wifi_timer = millis();
  }
  if (millis() - wifi_timer >= 60 * 1000)
  {
    Serial.println("[ESP] ПЕРЕЗАГРУЗКА ДЛЯ ПОВТОРНОГО ПОДКЛЮЧЕНЕИЯ");
    ESP.restart();
  }
}

void led_blink()
{
  if (millis() - led_timer >= 500)
  {
    led_timer = millis();
    digitalWrite(D4, !digitalRead(D4));
  }
}

// =================================================

void esp_restart()
{
  if (millis() >= RESTART_TIMER * 1000)
  {
    ESP.restart();
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(D4, OUTPUT);

  WIFI_connect();
  MQTT_connect();
}

void loop()
{
  client.loop();

  led_blink();
  esp_restart();

  get_temp();
  check_wifi();
  send_data();
}