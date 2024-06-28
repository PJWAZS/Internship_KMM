#include <Wire.h>
#include "Adafruit_SHT31.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();

void initSHT()  {
    Serial.println("get SHT31 Sensor");
    if (!sht31.begin(0x44))
    { 
        Serial.println("Couldn't find SHT31");
    }
}
