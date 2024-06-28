

void debug_GSM()
{

    Serial.println("debug_GSM()..");

    String imei = "";
    String imsi = "";
    int csq = 0;
    String ccid = "";
    IPAddress localip;
    String modemInfo;

    String modemName = modem.getModemName();
    delay(500);
    Serial.println("Modem Name: " + modemName);

    modemInfo = modem.getModemInfo();
    delay(2000);
    Serial.print("Modem Info: ");
    Serial.println(modemInfo);
    ccid = modem.getSimCCID();
    // DBG("CCID:", ccid);
    Serial.print("[CCID: ] ");
    Serial.println(ccid);
    imei = modem.getIMEI();
    // DBG("IMSI:", imsi);
    Serial.print("[IMEI: module ] ");
    Serial.println(imei);

    imsi = modem.getIMSI();
    // DBG("IMSI:", imsi);
    Serial.print("[IMSI: sim] ");
    Serial.println(imsi);

    String cop = modem.getOperator();
    // DBG("Operator:", cop);
    Serial.print("[Operator: ] ");
    Serial.println(cop);

    localip = modem.localIP();
    // DBG("Local IP:", local);
    Serial.print("[Local IP: ] ");
    Serial.println(localip);
    // showip(localip.toString());
    csq = modem.getSignalQuality();
    // DBG("Signal quality:", csq);
    Serial.print("[Signal quality: ] ");
    Serial.println(csq);
    Serial.println("+----------------------------------------+");
    // showcsq(csq);
}

void initModem()
{

    Serial.println("DONE Modem is ready!");
    Serial.println("Initializing modem...");

    if (!modem.init())
    {
        Serial.println("Failed initial modem");
    }

    Serial.print("Connecting to APN: ");
    Serial.print(apn);
    if (!modem.gprsConnect(apn, gprsUser, gprsPass))
    {
        Serial.println(" fail");
        // connectFail();
    }
    else
    {
        Serial.println(" OK");
        // connectSuccess();
    }

    debug_GSM();
}

void PowerOnModem()
{
    Serial.println("--- Power On Modem ");
    pinMode(MODEM_POWER_ON, OUTPUT);
    Serial.println("MODEM_POWER_ON ");
    digitalWrite(MODEM_POWER_ON, HIGH);
    delay(3000);
}

void EnableModem()
{
    Serial.println("--- Enable On Modem ");
    PowerOnModem();
    delay(10000);
    // check press on , press off
    Serial.println("----  Simulate push MODEM_KEY_ON");
    Serial.print("----  Check MODEM_STATUS : ");

    if (digitalRead(MODEM_STATUS) == 1)
    {

        Serial.println(digitalRead(MODEM_STATUS));
        digitalWrite(MODEM_KEY_ON, HIGH);
        delay(5000);
        digitalWrite(MODEM_KEY_ON, LOW);
    }
    else
    {
        Serial.println(digitalRead(MODEM_STATUS));
        Serial.println("Not Press ...");
    }
}

void modemPowerOff()
{
    Serial.println("--- Power Off Modem ");
    pinMode(MODEM_POWER_ON, OUTPUT);
    Serial.println("MODEM_POWER_ON ");
    digitalWrite(MODEM_POWER_ON, LOW);
    delay(3000);
}
void enableGPS(void)
{
    Serial.println("--- START GPS");
    modem.sendAT("+SGPIO=0,4,1,1");
    if (modem.waitResponse(10000L) != 1)
    {
        Serial.println("GPS FAIL");
        // Serial.println("SGPIO=0,4,1,1 false ");
    }
    else
    {
        Serial.println("GPS ON");
    }
    modem.enableGPS();
}

void getGPS()
{
    modem.enableGPS();

    uint8_t mode = modem.getGNSSMode();
    Serial.print("Current GNSS Mode (1=GPS 2 BDS 3=GPS + BDS 4=GLONASS 5=GPS + GLONASS 6=BDS + GLONASS 7=GPS + BDS + GLONASS): ");
    Serial.println(mode);

    uint16_t gnssMode = 7;
    Serial.println("Set GNSS Mode to : "+String(gnssMode));
    String res = modem.setGNSSMode(gnssMode, 1);
    Serial.println(res);
    delay(1000);

    delay(15000);
    float lat = 0;
    float lon = 0;
    float speed = 0;
    float alt = 0;
    int vsat = 0;
    int usat = 0;
    float accuracy = 0;
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int min = 0;
    int sec = 0;
    for (int8_t i = 15; i; i--)
    {
        Serial.println("Requesting current GPS/GNSS/GLONASS location");
        if (modem.getGPS(&lat, &lon, &speed, &alt, &vsat, &usat, &accuracy,
                         &year, &month, &day, &hour, &min, &sec))
        {
            Serial.println("Latitude: " + String(lat, 8) + "\tLongitude: " + String(lon, 8));
            Serial.println("Speed: " + String(speed) + "\tAltitude: " + String(alt));
            Serial.println("Visible Satellites: " + String(vsat) + "\tUsed Satellites: " + String(usat));
            Serial.println("Accuracy: " + String(accuracy));
            Serial.println("Year: " + String(year) + "\tMonth: " + String(month) + "\tDay: " + String(day));
            Serial.println("Hour: " + String(hour) + "\tMinute: " + String(min) + "\tSecond: " + String(sec));
            break;
        }
        else
        {
            Serial.println("Couldn't get GPS/GNSS/GLONASS location, retrying in 15s.");
            delay(15000L);
        }
    }
    Serial.println("Retrieving GPS/GNSS/GLONASS location again as a string");
    String gps_raw = modem.getGPSraw();
    Serial.println("GPS/GNSS Based Location String: " + gps_raw);
    Serial.println("Disabling GPS");
    modem.disableGPS();

    modem.sendAT("+SGPIO=0,4,1,0");
    if (modem.waitResponse(10000L) != 1)
    {
        Serial.println(" SGPIO=0,4,1,0 false ");
    }
}
