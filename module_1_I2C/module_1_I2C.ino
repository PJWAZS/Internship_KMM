#include <Wire.h>

void setup() {
  Wire.begin(); // เข้าร่วม bus I2C
  Serial.begin(115200); // เริ่มการสื่อสารแบบ serial ที่ baud rate 115200
  while (!Serial); // รอให้พอร์ต serial เปิด (จำเป็นสำหรับ Leonardo/Micro)
  Serial.println("\nI2C Scanner");
}

void loop() {
  byte error, address;
  int nDevices;
  
  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000); // รอ 5 วินาทีก่อนที่จะสแกนอีกครั้ง
}

