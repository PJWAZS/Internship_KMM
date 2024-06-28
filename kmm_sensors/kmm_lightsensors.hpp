#include <Wire.h>
//-------------------------------------------------------
#include "SparkFun_VEML6030_Ambient_Light_Sensor.h"

#define AL_ADDR 0x10
SparkFun_Ambient_Light light(AL_ADDR);

float gain = .125;
long luxVal = 0;

//-------------------------------------------------------
#include "VEML3328.h"
#define VEML3328_I2C_ADDRESS 0x10

VEML3328 RGBCIR;

uint16_t L_Blue = 0;
uint16_t L_Red = 0;
uint16_t L_Green = 0;
uint16_t L_Clear = 0;
uint16_t L_IR = 0;

void getVemlLight()
{
    if (light.begin())
        Serial.println("Ready to sense some light!");
    else
        Serial.println("Could not communicate with the sensor!");

    // Again the gain and integration times determine the resolution of the lux
    // value, and give different ranges of possible light readings. Check out
    // hoookup guide for more info.
    light.setGain(gain);
    // light.setIntegTime(time);

    Serial.println("Reading settings...");
    Serial.print("Gain: ");
    float gainVal = light.readGain();
    Serial.print(gainVal, 3);
    Serial.print(" Integration Time: ");
    int timeVal = light.readIntegTime();
    Serial.println(timeVal);

    delay(100);
    //-----------------------------------------------------------------------------------
    Serial.println();
    luxVal = light.readLight();
    Serial.print("Ambient Light Reading: ");
    Serial.print(luxVal);
    Serial.println(" Lux");
    delay(100);


    //-----------------------------------------------------------------------------------
    if (!RGBCIR.begin())
    {                                                        // chekc if the sensor is connected
        Serial.println("ERROR: couldn't detect the sensor"); // display an error message if the sensor is not connected
        while (1)
        {
        }
    }
    Serial.println("VEML3328 RGBCIR color sensor");
    RGBCIR.Enable();                    // enable the sensor channels
    RGBCIR.setGain(4);                  // set the amplification gain to 4 (0.5,1,2 can also be selected)
    RGBCIR.setSensitivity(high_sens);   // set the sensitivity mode (low_sens can also be selected)
    RGBCIR.setDG(4);                    // set the digital gain (1,2 can also be selected)
    RGBCIR.setIntegrationTime(IT_50MS); // set the sensor's integration time or the time it takes to take one measurement (IT_100MS, IT_200MS, IT_400MS can also be selected)


    // Use the arduino's serial plotter
    Serial.print("Blue:");
    Serial.print(RGBCIR.getBlue()); // read blue channel
    L_Blue = RGBCIR.getBlue();
    Serial.print(',');
    Serial.print("Red:");
    Serial.print(RGBCIR.getRed()); // read red channel
    L_Red = RGBCIR.getRed();
    Serial.print(',');
    Serial.print("Green:");
    Serial.print(RGBCIR.getGreen()); // read green channel
    L_Green = RGBCIR.getGreen();
    Serial.print(',');
    Serial.print("Clear:");
    Serial.print(RGBCIR.getClear()); // read clear channel
    L_Clear = RGBCIR.getClear();
    Serial.print(',');
    Serial.print("IR:");
    Serial.println(RGBCIR.getIR()); // read IR channel
    L_IR = RGBCIR.getIR();
    delay(100);
}




