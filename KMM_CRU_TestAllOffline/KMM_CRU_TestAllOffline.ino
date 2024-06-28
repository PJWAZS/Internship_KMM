/**************************************************************
   Project      KMM-CRU_TestAllOffline.ino
   Description  For test each module all KMM-CRU-TrainingBoard

   Programmer   Mr.Prarinya Ekapho
   Date         2023-11-06

   Hardware     ESP32 DOIT DEVKET V1 + KMM-CRU-TrainingBoard

 **************************************************************/
#include "sysCfg.h"
#include "MyClass.h"
#include "MyObjects.h"
#include "MyFunctions.h"
#include "MyTasks.h"

void setup()
{
  Wire.begin();        // Start the I2C interface
  lcd.begin();
  lcd.backlight();
  lcd.print(MyCompany);
  lcd.setCursor(0,1);
  lcd.print("Ver.");
  lcd.print(Version);
  Serial.begin(9600);
  Serial.println(MyCompany);
  Serial.print("Software Version ");
  Serial.println(Version);  
  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("KMM-CRU-TrainingBoard Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
  Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.

  IO_Initial();
  initSDcard();

  //ManualSW1: Checking button is pressed if yes make relay1 on
  xTaskCreate(&ManualSW1, "ManualSW1", 1000, NULL, 5, NULL);  
  //ManualSW2: Checking button is pressed if yes make relay2 on
  xTaskCreate(&ManualSW2, "ManualSW2", 1000, NULL, 5, NULL);
  //ManualSW3: Checking button is pressed if yes make relay3 on
  xTaskCreate(&ManualSW3, "ManualSW3", 1000, NULL, 5, NULL);
  //ManualSW4: Checking button is pressed if yes make relay4 on
  xTaskCreate(&ManualSW4, "ManualSW4", 1000, NULL, 5, NULL);
  xTaskCreate(&Heartbeat, "Heartbeat", 1000, NULL, 10, NULL); 
}

void loop()
{  
  getSensor();      //working on timer1 every 29000ms = 29s  
  ExtendDisplay();
  logSDCard();
  dispDT_LCD();
  dispLightLCD();
  dispTH_LCD();
  
}
