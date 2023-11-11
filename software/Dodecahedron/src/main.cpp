#define FASTLED_ESP32_I2S true
#define RELAY_PIN 33

#include "main.h"

#include <ArduinoHttpClient.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include "space/Animation.h"
/*---------------------------------------------------------------------------------------
 * Globals
 *-------------------------------------------------------------------------------------*/
// Global configuration parameters
Config config;
// Taskhandle for running tasks on both cores
TaskHandle_t Task;
void task(void *);
/*---------------------------------------------------------------------------------------
 * Initialize setup parameters
 *-------------------------------------------------------------------------------------*/
void setup() {
  // Enable console logging
  Serial.begin(115200);
  Serial.println("Booting...");
  // Setup WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(config.network.ssid, config.network.password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  Serial.println("Connected to Network...");
  ArduinoOTA.setPort(3232);
  ArduinoOTA.setHostname(config.network.hostname);
  ArduinoOTA
      .onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
          type = "sketch";
        else
          type = "filesystem";
        vTaskSuspend(Task);
        Serial.println("Start updating " + type);
      })
      .onEnd([]() { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      })
      .onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
          Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
          Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
          Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
          Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR)
          Serial.println("End Failed");
      });
  ArduinoOTA.begin();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  // Initialize animation and display
  Animation::begin();
  // Create task1 on core 0
  xTaskCreatePinnedToCore(task, "API", 10000, NULL, 10, &Task, 0);
}
/*---------------------------------------------------------------------------------------
 * Loop Core 1 -> Animation and OTA
 *-------------------------------------------------------------------------------------*/
void loop() {
  static Timer fpsTimer = 1.0f;
  Serial.printf("Loop: Running on core %d\n", xPortGetCoreID());
  while (true) {
    // Handle OTA update
    ArduinoOTA.handle();
    // Run animation rountines and update the display
    Animation::animate();
    // Print FPS once every second
    if (fpsTimer.update()) {
      Serial.printf("FPS=%1.2f\n", Animation::fps());
    }
  }
}
/*---------------------------------------------------------------------------------------
 * Task Core 0 -> HTTP and MQTT
 *-------------------------------------------------------------------------------------*/
void task(void *parameter) {
  Serial.printf("Task running on core %d\n", xPortGetCoreID());
  DynamicJsonDocument doc(2048);
  WiFiClient wifi_1, wifi_2;
  HttpClient httpclient = HttpClient(wifi_1, config.network.hue_ip, 80);
  PubSubClient mqttclient(wifi_2);
  mqttclient.setServer(config.network.broker_ip, 1883);
  String hue_api = config.network.hue_api;

  mqttclient.setCallback([](char *topic_, byte *payload, unsigned int length) {
    String response, topic(topic_);
    for (int i = 0; i < length; i++) response += (char)payload[i];
    if (topic.equals(config.network.mqtt_topics.onoff)) {
      if (response == "true") {
        pinMode(RELAY_PIN, OUTPUT);
        digitalWrite(RELAY_PIN, HIGH);
      } else if (response == "false") {
        pinMode(RELAY_PIN, OUTPUT);
        digitalWrite(RELAY_PIN, LOW);
      }
    } else if (topic.equals(config.network.mqtt_topics.dim)) {
      config.network.mqtt_values.dim = 2.55f * response.toInt();
    } else if (topic.equals(config.network.mqtt_topics.color)) {
      uint8_t hue = (255 * response.toInt()) / 360;
      response = response.substring(response.indexOf(',') + 1);
      uint8_t sat = (255 * response.toInt()) / 100;
      config.network.mqtt_values.color = CRGB(CHSV(hue, sat, 255));
    } else if (topic.equals(config.network.mqtt_topics.scene)) {
      config.network.mqtt_values.scene = response.toInt();
    }
  });

  auto handleHTTP = [&](decltype(config.lights.light[0]) &light) {
    int httpcode = httpclient.get((hue_api + light.name).c_str());
    if (httpcode == HTTP_SUCCESS) {
      DeserializationError err =
          deserializeJson(doc, httpclient.responseBody());
      if (err)
        Serial.printf("Deserialization error: %s\n", err.c_str());
      else {
        light.on = doc["state"]["on"];
        light.bri = doc["state"]["bri"];
        light.hue = doc["state"]["hue"];
        light.sat = doc["state"]["sat"];
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %d\n",
                    httpclient.responseStatusCode());
    }
  };

  auto handleMQTT = [&]() {
    if (!mqttclient.connected()) {
      if (mqttclient.connect(config.network.hostname)) {
        mqttclient.subscribe(config.network.mqtt_topics.onoff);
        mqttclient.subscribe(config.network.mqtt_topics.dim);
        mqttclient.subscribe(config.network.mqtt_topics.color);
        mqttclient.subscribe(config.network.mqtt_topics.scene);
      } else {
        Serial.printf("Can't Subcribe\n");
      }
    }
    if (mqttclient.connected()) {
      mqttclient.loop();
    }
  };

  while (true) {
    if (WiFi.status() != WL_CONNECTED) {
      WiFi.reconnect();
    } else {
      for (int i = 0; i < config.lights.lights; i++) {
        handleHTTP(config.lights.light[i]);
        vTaskDelay(1);
        handleMQTT();
        vTaskDelay(1);
      }
    }
  }
}