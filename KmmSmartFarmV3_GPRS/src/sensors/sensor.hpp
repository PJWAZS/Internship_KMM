#include <Arduino.h>
#include <ArduinoJson.h>
#include "SoilTemp.hpp"
#include "LightDFRobot.hpp"
#include "sht4x.hpp"
#include "VemlLight.hpp"
#include "Ecowitt.hpp"

float SoilHumidData = 0;
float SoilTempData = 0;
int Light = 0;
SoilTemp node3(3, "node3", &SerialSensor, 1.0f);
Ecowitt weather(144, &Serial);

void getData()
{
    node3.init();
    node3.getString();
    SoilHumidData = node3.getSoilHumid();
    SoilTempData = node3.getSoilTemp();

    getVemlLight();
    // sht40 sensors
    setupSHT4x();
    getSHT4xV2();
    Serial.println("Weather Ecowitt Data");
    weather.init();
    weather.readSensorData();
    weather.printData();
    String jsonData = weather.getDataAsJson();
}

void sensorOutHass()
{
    Serial.println("[Sensor] sensor Out Hass");
    const size_t capacity1 = JSON_OBJECT_SIZE(5) + 200;
    DynamicJsonDocument doc(capacity1);
    doc["air_humidity"] = humidity;
    doc["air_temperature"] = temperature;
    doc["lux"] = luxVal;
    doc["soil_humidity"] = SoilHumidData;
    doc["soil_temperature"] = SoilTempData;

    char buffer[256];
    size_t n = serializeJson(doc, buffer);
    Serial.print("  Debug\t ");
    Serial.println(buffer);
    Serial.print("  SENSOR _TOPIC Sensor\t");
    Serial.println(SENSOR_TOPIC);
    client.publish(SENSOR_TOPIC, buffer);

    weather.init();
    weather.readSensorData();
    weather.printData();
    // get the sensor data asa json and print the debug 
    String jsonData = weather.getDataAsJson();
    Serial.print("  SENSOR _TOPIC Weather\t");
    Serial.println(SENSOR_TOPIC);
    client.publish(SENSOR_TOPIC, jsonData.c_str());


}