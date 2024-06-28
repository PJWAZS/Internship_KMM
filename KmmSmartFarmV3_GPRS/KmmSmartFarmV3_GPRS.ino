
#include "src/config/config.h"
#include "src/global.h"
#include "src/config/gsm.h"
#include "src/config/rtctime.hpp"
#include "src/sensors/sensor.hpp"

unsigned long updateCurrentMillis;
unsigned long lastUpdateDelay;
unsigned long updateDelay = 300000;

void setup()
{
    Serial.begin(9600);
    SerialAT.begin(UART_BAUD, SERIAL_8N1, RXD1, TXD1);
    SerialSensor.begin(9600, SERIAL_8N1, RXD0, TXD0);
    getChipId();
    debugTopic();
    scani2c();
    setupGpio();
    testGPIO();

    oneBeeb();

    rtc_setup(true);
    rtc_showtime();

    EnableModem();
    Serial.println("Wait 30 Sec ... ");
    delay(30000);
    Serial.print("MODEM_STATUS (expected 0):  ");
    Serial.println(digitalRead(MODEM_STATUS));
    twoBeep();
    initModem();

    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);

    getData();
}

void loop()
{
    // getGPS();
    delay(2000);

    if (!client.connected())
    {
        Serial.println("  [Run] client.connected()");
        long now = millis();
        if (now - lastReconnectAttempt > 5000)
        {
            lastReconnectAttempt = now;
            // Attempt to reconnect
            if (reconnect())
            {
                lastReconnectAttempt = 0;
            }
        }
    }
    else
    {
        // Client connected
        client.loop();
    } // end

    delay(100);
    updateCurrentMillis = millis();
    if (updateCurrentMillis - lastUpdateDelay >= updateDelay)
    {
        // lcd_showip();
        getData();
        sensorOutHass();
        // uploadToThingsboard();
        lastUpdateDelay = updateCurrentMillis;
    } // end
}
