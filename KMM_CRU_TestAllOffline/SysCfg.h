#ifndef sysCfg_h
#define sysCfg_h

#define MyCompany "KoMoMi Co.,Ltd."
#define Version "1.0.0"

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

//SD card Libraries
#include "FS.h"
#include "SD.h"
#include <SPI.h>

#define EEPROM_ADDR 0x50  // I2C Buss address of 24LC256 256K EEPROM

#include <Wire.h>
#include <DS3231.h>
// #include <NoDelayTimerTask.h>
#include <ModbusMaster.h>
ModbusMaster node;

RTClib myRTC;
String dataMessage;
String xDate = "";
String xTime = "";
int hrs,mins;

//Set timer
NoDelayTimerTask timer1(5000), timer2(29000), timer3(59000), timer4(7000), timer5(13000);
//timer1 -> getSensor()
//timer2 -> logSDCard()
//timer3 -> dispDT_LCD()
//timer4 -> dispTH_LCD()
//timer5 -> dispLightLCD()

#include <Adafruit_MCP23X08.h>
#include <Adafruit_MCP23X17.h>
#include <Adafruit_MCP23XXX.h>

// Basic pin writing test for the MCP23017 I/O expander
// Connect pin #12 of the expander to I2C_SCL(GPI22) (i2c clock) of ESP32DEVKIT
// Connect pin #13 of the expander to I2C_SDA(GPI21) (i2c data) of ESP32DEVKIT
// Connect pins #15, 16 and 17 of the expander to ground 0x20 (address selection)
// Connect pin #9 of the expander to 5V (power)
// Connect pin #10 of the expander to ground (common ground)
// Connect pin #18 through a ~10kohm resistor to 5V (reset pin, active low)

// Output #0 - #7 is on pin 21 - 28 (GPAx; x = 0 - 7)so connect an LED or whatever from that to ground
// Output #8 - #15 is on pin 1 - 8 (GPBx; x = 8 - 15)so connect an LED or whatever from that to ground
Adafruit_MCP23X17 mcp;
int pin_led1 = 0; // IC pin 21 (GPA0)
int pin_led2 = 1; // IC pin 22 (GPA1)
int pin_led3 = 2; // IC pin 23 (GPA2)
int pin_led4 = 3; // IC pin 24 (GPA3)
int pin_led5 = 4; // IC pin 25 (GPA4)
int pin_led6 = 5; // IC pin 26 (GPA5)
int pin_led7 = 6; // IC pin 27 (GPA6)
int pin_led8 = 7; // IC pin 28 (GPA7)

//#ifndef LED_BUILTIN
#define LED_BUILTIN       2         // Pin D2 mapped to pin GPIO2/ADC12 of ESP32, control on-board LED
//#endif

uint64_t chipid; 
uint16_t pTemp=0;
uint16_t pHumi=0; 

String t1 = "00";
String h1 = "00";
String ls01 = "00";

int HB_period = 500;

// ***** GPIO PIN SETUP *****
#define RLY1    17  //Set Relay1 pin GPIO 17
#define RLY2    16  //Set Relay2 pin GPIO 16
#define RLY3    14   //Set Relay3 pin GPIO 14
#define RLY4    15  //Set Relay4 pin GPIO 15

#define Buzzer  33  //Set Buzzer pin GPIO 33

#define SW1   39    //Set Switch1 pin GPIO 39
#define SW2   34    //Set Switch2 pin GPIO 34
#define SW3   35    //Set Switch3 pin GPIO 35
#define SW4   27    //Set Switch3 pin GPIO 27

/*
    ESP32 Internal Temperature Sensor Example
*/
#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif

uint8_t temprature_sens_read();
float cTemp;

#endif
