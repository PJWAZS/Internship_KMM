#ifndef ECOWITT_H
#define ECOWITT_H

#include <Arduino.h>
#include <ModbusMaster.h>

#include <ModbusMaster.h>
#include <ArduinoJson.h>

class Ecowitt
{
private:
  uint8_t _id;
  ModbusMaster node;
  Stream *_Serial;
  uint8_t result;
  int16_t data;

  float convertTemperature(int16_t raw)
  {
    return (raw - 400) * 0.1;
  }

  float convertHumidity(int16_t raw)
  {
    return raw;
  }

  float convertWindspeed(int16_t raw)
  {
    return raw * 0.1;
  }

  float convertRainfall(int16_t raw)
  {
    return raw * 0.1;
  }

  float convertPressure(int16_t raw)
  {
    return raw * 0.1;
  }

public:
  Ecowitt(uint8_t id, Stream *Serial)
  {
    _id = id;
    _Serial = Serial;
  }

  void init()
  {
    node.begin(_id, *_Serial);
    delay(250);
  }

  void readSensorData()
  {
    result = node.readHoldingRegisters(0x0160, 14);
    if (result != 0)
    {
      Serial.print("Error reading sensor data: ");
      Serial.println(result);
    }
  }

  void printData()
  {
    if (result != 0)
      return; // Skip printing if there was an error

    Serial.print("\n  Data model  \t");
    Serial.print(node.getResponseBuffer(0x00));
    Serial.print("\n  Buad Rate \t");
    Serial.print(node.getResponseBuffer(0x01));
    Serial.print("\n  Address  \t");
    Serial.print(node.getResponseBuffer(0x02));
    Serial.print("\n  MSB  \t");
    Serial.print(node.getResponseBuffer(0x03));
    Serial.print("\n  LSB  \t");
    Serial.print(node.getResponseBuffer(0x04));
    Serial.print("\n  Light  \t");
    Serial.print(node.getResponseBuffer(0x05));
    Serial.print("\n  UV index  \t");
    Serial.print(node.getResponseBuffer(0x06));
    Serial.print("\n  Temperature  \t");
    Serial.print(convertTemperature(node.getResponseBuffer(0x07)));
    Serial.print("\n  Humidity \t");
    Serial.print(convertHumidity(node.getResponseBuffer(0x08)));
    Serial.print("\n  Windspeed \t");
    Serial.print(convertWindspeed(node.getResponseBuffer(0x09)));
    Serial.print("\n  Gustspeed \t");
    Serial.print(convertWindspeed(node.getResponseBuffer(0x0A)));
    Serial.print("\n  WindDirection \t");
    Serial.print(node.getResponseBuffer(0x0B));
    Serial.print("\n  RainFall \t");
    Serial.print(convertRainfall(node.getResponseBuffer(0x0C)));
    Serial.print("\n  ABS Pressure \t");
    Serial.print(convertPressure(node.getResponseBuffer(0x0D)));
  }

  String getDataAsJson()
  {
    Serial.println();
    Serial.println("[Weather] getDataAsJson");
    const size_t capacity = JSON_OBJECT_SIZE(14) + 200;
    DynamicJsonDocument doc(capacity);

    doc["DataModel"] = node.getResponseBuffer(0x00);
    doc["BaudRate"] = node.getResponseBuffer(0x01);
    doc["Address"] = node.getResponseBuffer(0x02);
    doc["MSB"] = node.getResponseBuffer(0x03);
    doc["LSB"] = node.getResponseBuffer(0x04);
    doc["Light"] = node.getResponseBuffer(0x05);
    doc["UVIndex"] = node.getResponseBuffer(0x06);
    doc["Temperature"] = convertTemperature(node.getResponseBuffer(0x07));
    doc["Humidity"] = convertHumidity(node.getResponseBuffer(0x08));
    doc["Windspeed"] = convertWindspeed(node.getResponseBuffer(0x09));
    doc["Gustspeed"] = convertWindspeed(node.getResponseBuffer(0x0A));
    doc["WindDirection"] = node.getResponseBuffer(0x0B);
    doc["Rainfall"] = convertRainfall(node.getResponseBuffer(0x0C));
    doc["Pressure"] = convertPressure(node.getResponseBuffer(0x0D));

    // Calculate the exact size needed for the JSON buffer
    size_t jsonSize = measureJson(doc);
    char buffer[jsonSize + 1]; // +1 for null-terminator
    serializeJson(doc, buffer, sizeof(buffer));
    Serial.print("  Debug\t ");
    Serial.println(buffer);

    return String(buffer);
  }
};

#endif