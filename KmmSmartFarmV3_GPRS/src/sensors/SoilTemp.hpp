#include <Arduino.h>
#include <ModbusMaster.h>

class SoilTemp
{
private:
    uint8_t _id;
    String _name;
    ModbusMaster node;
    Stream *_Serial;
    uint8_t j, result;
    uint16_t rawdata[9];
    int16_t data;
    float _factor;

    int16_t soilhumid = 0; //  10
    int16_t soiltemp = 0;  //  10
    // int16_t soilec = 0;    //  1000

    float soilhumid_float = 0; //  10
    float soiltemp_float = 0;  //  10
    //float soilec_float = 0;    //  1000

public:
    // contructor
    SoilTemp(uint8_t id, String name, Stream *Serial, float factor)
    {
        _id = id;
        _name = name;
        _Serial = Serial;
        _factor = factor;
    }

    void init()
    {
        Serial.println("┌------------------------------------------------┐");
        Serial.println("│ ** init Soil Temp                                  │");
        Serial.println("└------------------------------------------------┘");
        node.begin(_id, *_Serial);
        int n = 0;
        int num_try = 10;
        do
        {
            delay(500);
            result = node.readHoldingRegisters(0x0000, 3); // 1 st 16 bit
            delay(500);
            Serial.print(" result: ");
            Serial.println(result);
            if (result == node.ku8MBSuccess)
            {
                Serial.println("[+] Success to read");
                // get data
                for (j = 0; j < 1; j++)
                {
                    rawdata[j] = node.getResponseBuffer(j);
                    Serial.println(node.getResponseBuffer(j));
                }
                data = node.getResponseBuffer(0x00);
                soilhumid = data;
                soilhumid_float = soilhumid;

                data = node.getResponseBuffer(0x01);
                soiltemp = data;
                soiltemp_float = soiltemp;

                // data = node.getResponseBuffer(0x02);
                // soilec = data;
                // soilec_float = soilec;
                break;
            }
            else
            {
                Serial.println("[+] Fail to read Soil Temp");
                soiltemp = 0;
            }
            n++;
        } while (n < num_try);

        char buffer[100];
        if (n > 1)
        {
            sprintf(buffer, "Try  Soil Temp %d time", n);
            Serial.println(buffer);
        }
    }

    void getString()
    {
        Serial.println("┌------------------------------------------------┐");
        Serial.println("│ >> Get string                                  │");
        Serial.println("└------------------------------------------------┘");
        Serial.print("[+] Id: ");
        Serial.println(_id);
        Serial.print("[+] _name: ");
        Serial.println(_name);
        Serial.println("");
        Serial.print("soilhumid ");
        Serial.println(soilhumid);
        Serial.println(soilhumid_float / 10);

        Serial.print("soiltemp ");
        Serial.println(soiltemp);
        Serial.println(soiltemp_float / 10);

        // Serial.print("soilec");
        // Serial.println(soilec);
        // Serial.println(_factor * soilec_float / 1000);
    }

    float getSoilTemp()
    {
        return float(soiltemp_float / 10);
    }

    float getSoilHumid()
    {
        return float(soilhumid_float / 10);
    }

    // String getSoilEc()
    // {
    //     return String(_factor * soilec_float / 1000);
    // }
};

// brown  +
// black  -
// yellow A
// blue B

// Address
// 1 - 9        อภหภูมิความชื้นดิน
// 10 - 19      อุฎหมูความชื้นอากาศ
// 20 - 29      แสง
// 30 - 39      แร่ธาตุ ในดิน