#include <Wire.h>
#include "RTClib.h"
#include <TimeLib.h> // https://github.com/PaulStoffregen/Time

RTC_DS3231 rtc;
DateTime nowRTC;
time_t nowTime;
bool isRTCrunning = false;

time_t
syncProvider() // this does the same thing as RTC_DS1307::get()
{
    return rtc.now().unixtime();
}

void scani2c()
{
    byte error, address;
    int nDevices;
    Wire.begin();
    Serial.println(F("Scanning...RTC Device"));

    nDevices = 0;
    for (address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0)
        {
            Serial.print(F("I2C device found at address 0x"));
            if (address < 16)
            {
                Serial.print("0");
            }
            Serial.println(address, HEX);
            nDevices++;
        }
        else if (error == 4)
        {
            Serial.print(F("Unknow error at address 0x"));
            if (address < 16)
            {
                Serial.print("0");
            }
            Serial.println(address, HEX);
        }
    }
    if (nDevices == 0)
    {
        Serial.println(F("No I2C devices found\n"));
    }
    else
    {
        Serial.println(F("done\n"));
    }
    delay(5000);
}
/*
 * rtc_setup(true) run first time to setup RTC
 * rtc_setup(false)
 */
void rtc_setup(bool mode)
{
    Serial.println("2 rtc_setup Set time ");
    Wire.begin();
    rtc.begin();
    if (mode)
    {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    setSyncProvider(syncProvider);
}

void rtc_showtime()
{
    Serial.println("3 rtc_showtime Show time ");
    nowRTC = rtc.now();
    nowTime = nowRTC.unixtime();
    Serial.print("UnixTime : ");
    Serial.println(nowTime);

    Serial.print(nowRTC.year(), DEC);
    Serial.print('/');
    Serial.print(nowRTC.month(), DEC);
    Serial.print('/');
    Serial.print(nowRTC.day(), DEC);
    Serial.print(' ');
    Serial.print(nowRTC.hour(), DEC);
    Serial.print(':');
    Serial.print(nowRTC.minute(), DEC);
    Serial.print(':');
    Serial.print(nowRTC.second(), DEC);
    Serial.println();

    // debug TimeLib.h
    /*For Debugging*/
    Serial.print("The Time is: ");
    Serial.print(hour());
    Serial.print(" : ");
    Serial.print(minute());
    Serial.print(" : ");
    Serial.println(second());
    Serial.print("The Date is: ");
    Serial.print(day());
    Serial.print(".");
    Serial.print(month());
    Serial.print(".");
    Serial.println(year());

    char buffer[80];
    sprintf(buffer, "%4d-%02d-%02d %02d:%02d:%02d", year(), month(), day(), hour(), minute(), second());
    Serial.println(buffer);
    delay(1000);
}

String rtc_dayStamp()
{
    Serial.println(F("Rtc DayStamp"));
    DateTime now = rtc.now();
    char buffer[50];
    sprintf(buffer, "%d-%d-%d", now.year(), now.month(), now.day());
    // Serial.println(buffer);
    return String(buffer);
}

String rtc_timeStamp()
{
    // Serial.println(F("rtc timeStamp"));
    DateTime now = rtc.now();
    char buffer[50];
    sprintf(buffer, "%d:%d:%d", now.hour(), now.minute(), now.second());
    // Serial.println(buffer);
    return String(buffer);
}