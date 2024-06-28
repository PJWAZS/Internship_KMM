#include <Arduino.h>
#include <WiFi.h>
#include <Arduino_JSON.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_SHT4x.h>
#include <PubSubClient.h>

// Replace with your network credentials
const char* ssid = "MONIC";
const char* password = "1000603413";

#define MQTT_PUB "sensorReadings" 

const char* mqttServer = "172.20.10.4"; // Your Ubuntu IP address
const int mqttPort = 1883;

// Create a sensor object
Adafruit_SHT4x sht4 = Adafruit_SHT4x();

// MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

// Json Variable to Hold Sensor Readings
JSONVar readings;

float temperature = 0;
float humidity = 0;

// Init SHT4x
void initSHT4x() {
  if (!sht4.begin()) {
    Serial.println("Couldn't find SHT4x sensor, check wiring!");
    while (1) delay(1);
  }
  Serial.println("Found SHT4x sensor");
  Serial.print("Serial number 0x");
  Serial.println(sht4.readSerial(), HEX);
}

// Get Sensor Readings and return JSON object
String getSensorReadings() {
  sensors_event_t humidity_event, temp_event;
  sht4.getEvent(&humidity_event, &temp_event);

  readings["temperature"] = String(temp_event.temperature);
  readings["humidity"] = String(humidity_event.relative_humidity);

  String jsonString = JSON.stringify(readings);
  return jsonString;
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

// Initialize MQTT
void initMQTT() {
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  initSHT4x();
  initWiFi();
  initMQTT();
}

void loop() {
  if (!client.connected()) {
    initMQTT();
  }
  client.loop();

  if ((millis() - lastTime) > timerDelay) {
    String sensorReadings = getSensorReadings();
    Serial.println(sensorReadings);
    client.publish(MQTT_PUB, sensorReadings.c_str());
    lastTime = millis();
  }
}
