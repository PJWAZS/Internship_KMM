#ifndef MyFunctions_h
#define MyFunctions_h

void IO_Initial();
void ExtendDisplay();
void getSensor();
void printValues();
void getTHSensors();
void ChipTemp();
void BuzzerSound();
void initSDcard();
void appendFile(fs::FS &fs, const char * path, const char * message);
void writeFile(fs::FS &fs, const char * path, const char * message);
void logSDCard();
void ChipTemp();
void getLightSensors();
void getLightSensors();
void dispDT_LCD();
void dispLightLCD();


void BuzzerSound() {
  digitalWrite(Buzzer,LOW);
  for(int i = 0; i < 300; i++) delay(1);
  digitalWrite(Buzzer,HIGH);
}


void IO_Initial(){
  pinMode(RLY1, OUTPUT);
  pinMode(RLY2, OUTPUT);
  pinMode(RLY3, OUTPUT);
  pinMode(RLY4, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(Buzzer, OUTPUT);

  digitalWrite(Buzzer,HIGH);

  mcp.begin_I2C(); // use default address 0
  mcp.pinMode(pin_led1, OUTPUT);
  mcp.pinMode(pin_led2, OUTPUT);
  mcp.pinMode(pin_led3, OUTPUT);
  mcp.pinMode(pin_led4, OUTPUT);
  mcp.pinMode(pin_led5, OUTPUT);
  mcp.pinMode(pin_led6, OUTPUT);
  mcp.pinMode(pin_led7, OUTPUT);
   
  
  pinMode(SW1,INPUT);
  pinMode(SW2,INPUT);
  pinMode(SW3,INPUT);
  pinMode(SW4,INPUT);  

  mcp.digitalWrite(pin_led1,HIGH);  //Power on
  mcp.digitalWrite(pin_led2,LOW);
  mcp.digitalWrite(pin_led3,LOW);
  mcp.digitalWrite(pin_led4,LOW); 
  mcp.digitalWrite(pin_led5,LOW);
  mcp.digitalWrite(pin_led6,LOW);
  mcp.digitalWrite(pin_led7,LOW);
   

  R1.RelayOff();
  R2.RelayOff();
  R3.RelayOff();
  R4.RelayOff();
}

void ExtendDisplay(){
  int val;
  if (val = digitalRead(LED_BUILTIN)== 0){
    mcp.digitalWrite(pin_led2,LOW);
  } else {
    mcp.digitalWrite(pin_led2,HIGH);
  }
  if (R1.getStatus()==0) {
    mcp.digitalWrite(pin_led4,LOW);
  } else {
    mcp.digitalWrite(pin_led4,HIGH);
  }
  if (R2.getStatus()==0) {
    mcp.digitalWrite(pin_led5,LOW);
  } else {
    mcp.digitalWrite(pin_led5,HIGH);
  }
  if (R3.getStatus()==0) {
    mcp.digitalWrite(pin_led6,LOW);
  } else {
    mcp.digitalWrite(pin_led6,HIGH);
  }
  if (R4.getStatus()==0) {
    mcp.digitalWrite(pin_led7,LOW);
  } else {
    mcp.digitalWrite(pin_led7,HIGH);
  }   
}

void getSensor(){
  if(timer1.isTimeUp())
  {
    getTHSensors(); //get valve from sensor
    getLightSensors();
//    getSoilData();
//    getWeatherData();
//    chkConditionTemp();
    printValues();    //Show on serial monitor
    timer1.resetTimer();  
  }
}

void dispDT_LCD() {
  if(timer3.isTimeUp())
  {
    lcd.clear();
    lcd.print("Date: " + xDate);   
    lcd.setCursor(0,1);
    lcd.print("Time: " + xTime);  
    timer3.resetTimer(); 
  }
}

void dispLightLCD() {
  if(timer5.isTimeUp())
  {
    lcd.clear();
    lcd.print("Light: (Lux)");
    lcd.setCursor(0,1);
    lcd.print(ls01);
    timer5.resetTimer();
  }  
}

void dispTH_LCD() {
  if(timer4.isTimeUp())
  {
    lcd.clear();
    lcd.print("Humi: ");
    lcd.print(h1);
    lcd.print("%");
    lcd.setCursor(0,1);
    lcd.print("Temp: ");
    lcd.print(t1);
    lcd.print((char) 223);
    lcd.print("C");
    timer4.resetTimer();
  }
}

void printValues() {
  BuzzerSound();
  DateTime now = myRTC.now();
  char str[20];
  sprintf(str, "%02d/%02d/%02d %02d:%02d:%02d",  now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second());
  Serial.print(F("Date/Time: "));
  Serial.println(str);
  sprintf(str, "%02u/%02u/%04u", now.day(), now.month(), now.year());
  xDate = str;  
//  lcd.clear();
//  lcd.print("Date: " + xDate);
  sprintf(str, "%02u:%02u", now.hour(), now.minute());
  xTime = str;
//  lcd.setCursor(0,1);
//  lcd.print("Time: " + xTime);
//  for(int i = 0; i < 2000; i++) delay(1);
  //get sensor value
  ChipTemp();
  Serial.println("Light (LS-01): " + ls01 + "Lux");  
  Serial.print("Humidity (TH-01): " + h1 + "%");
  Serial.println("\tTemperature (TH-01): " + t1 + "°C");

//  lcd.clear();
//  lcd.print("Humi: ");
//  lcd.print(h1);
//  lcd.print("%");
//  lcd.setCursor(0,1);
//  lcd.print("Temp: ");
//  lcd.print(t1);
//  lcd.print((char) 223);
//  lcd.print("C");  
}

void getTHSensors() {
    String tmp;  
    tmp = TH01.getHumiTemp();
    h1 = tmp.substring(0, tmp.indexOf(','));
    t1 = tmp.substring(tmp.indexOf(',') + 1);        
}

void getLightSensors() {  
     
    ls01 = LS01.getLight();   
         
//    Serial.println("Light (LS-01): " + ls01 + "Lux");                
}

void ChipTemp() {
    Serial.print("Chip Temperature: ");
    // Convert raw temperature in F to Celsius degrees
    cTemp = (temprature_sens_read() - 32) / 1.8;
    Serial.print(cTemp);
    //    Serial.print((temprature_sens_read() - 32) / 1.8);
    Serial.println("°C");
//    lcd.clear();
//    lcd.print("Chip Temperature");
//    lcd.setCursor(0,1);
//    lcd.print(cTemp);
//    lcd.print("C");
}

// Write to the SD card (DON'T MODIFY THIS FUNCTION)
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// Append data to the SD card (DON'T MODIFY THIS FUNCTION)
void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

// Write the sensor readings on the SD card
void logSDCard() {
  if(timer2.isTimeUp())
  {  
    dataMessage = String(xDate) + "," + String(xTime) + 
                  String(cTemp) + "," + t1 + "," + h1 + ls01 + "\r\n";

    Serial.print("Save data: ");
    Serial.println(dataMessage);
    lcd.clear();
    lcd.print("Writing to SD");
    for(int i=0;i<1000;i++) delay(1);
    appendFile(SD, "/datalog.txt", dataMessage.c_str());
    
  timer2.resetTimer();  
  }
}

void initSDcard() {
  // Initialize SD card
  SD.begin();  
  if(!SD.begin()) {    
    Serial.println("Card Mount Failed");
    lcd.clear();
    lcd.print("Card Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    lcd.clear();
    lcd.print("No SD card");
    return;
  }
  Serial.println("Initializing SD card...");
  if (!SD.begin()) {    
    Serial.println("ERROR - SD card initialization failed!");
    lcd.clear();
    lcd.print("ERROR - SD card");
    return;    // init failed
  }

  // If the datalog.txt file doesn't exist
  // Create a file on the SD card and write the data labels
  File file = SD.open("/datalog.txt");
  if(!file) {
    Serial.println("File doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/datalog.txt", "Date, Hour, ChipTemp, Temp, Humi, Light \r\n");
  }
  else {
    Serial.println("File already exists");  
  }
  file.close();  
}

#endif
