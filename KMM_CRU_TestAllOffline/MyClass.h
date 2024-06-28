#ifndef MyClass_h
#define MyClass_h

//---------- Switch Object ------------------
class switchCtl {
    bool status = 1;
    int pin = 0;

  public:
    switchCtl(int i ) {
      pin = i;
      pinMode(pin, INPUT);
    }

    int getStatus() {
      this->status = digitalRead(pin);
      return this->status;
    }
};

//----------- Humidity and Temperature Sensor Object -------------
class HumiTemp {
    int _id;
    String Temperature = "00.0";
    String Humidity = "00.0";
    uint8_t result;
    uint16_t data;

  public:
    HumiTemp(int i) {
      _id = i;
      getData();
    };

    String getId() {
      return String(this->_id) ;
    }

    String getTemperature() {
      getData();
      return this->Temperature ;
    }

    String getHumidity() {
      getData();
      return this->Humidity;
    }

    String getHumiTemp() {
      getData();
      return this->Humidity + "," + this->Temperature;
    }


    void getData() {
      node.begin(_id, Serial);
      // Reading sensor modbus device , RS1-0102
      result = node.readHoldingRegisters(0x0000, 2);
      for(int i=0;i<50;i++) delay(1);
//      delay(50);
      if (result == node.ku8MBSuccess) {
        // Temperature value Device ID#1
        data  = node.getResponseBuffer(0x00);
        if(data <= 1000) {
          pTemp = data;
        } else {
          data = pTemp;
        }
        this->Temperature = String(data / 10) + "." + String(data % 10);
        data  = node.getResponseBuffer(0x01);
        if(data <= 1000) {
          pHumi = data;
        } else {
          data = pHumi;
        }
        this->Humidity = String(data / 10) + "." + String(data % 10);
      } else {
        Serial.println("Temp/Humi Sensor not found... ");
        Temperature = "00.0";
        Humidity = "00.0";
      }
    }
};

//---------- Ambient Light Sensor Object ----------------
class AmbientLight {
  private:
    int _id;
    String lux = "0";
    unsigned long Ambient_Light;
    uint8_t result;
    uint16_t data;
  public:
    AmbientLight(int i) {
      this->_id = i;
      getData();
    }

    void getData() {

      node.begin(_id, Serial);
      // Reading sensor modbus device , RS1-0401
      result = node.readHoldingRegisters(0x0000, 2);
      for(int i=0;i<150;i++) delay(1);
//      delay(50);
      if (result == 0) {
        // Ambient light sensor is long data type. Double word size from register word 0 and 1
        this->Ambient_Light = ((unsigned long)(node.getResponseBuffer(0x00)) << 16) | node.getResponseBuffer(0x01);
        lux = String(Ambient_Light);
      } else {
        Serial.println("Light Sensor not found... ");
        lux = "0";
      }
    }

    String getLight() {
      getData();
      return this->lux;
    }
};

//------------- Relay Object --------------
class Relay {
  private:
    int Rstatus = 0;
    int pin;
  public:
    Relay(int i = 0) {
      this->pin = i;
      pinMode(this->pin, OUTPUT);
      digitalWrite(this->pin, HIGH);
    }

    int getStatus() {
      return this->Rstatus ;
    }

    int getPin() {
      return this->pin;
    }

    void setPin(int p) {
      pin = p;
    }

    void RelayOn() {
      digitalWrite(this->pin, LOW);
      this->Rstatus = 1;
    }

    void RelayOff() {
      digitalWrite(this->pin, HIGH);
      this->Rstatus = 0;
    }
};

#endif
