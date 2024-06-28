#include <ArduinoJson.h>
#include <Simpletimer.h>
//
#include "komomiconnect.hpp"
#include "blynkconnect.hpp"
#include "tbconnect.hpp"
#include "lineconnect.hpp"
#include "kmm_lightsensors.hpp"
#include "kmm_sht4x.hpp"

//
Simpletimer timer1{};

// ***** GPIO PIN SETUP *****
#define RLY1    17  // Set Relay1 pin GPIO 17
#define RLY2    16  // Set Relay2 pin GPIO 16
#define RLY3    14  // Set Relay3 pin GPIO 14
#define RLY4    15  // Set Relay4 pin GPIO 15

#define Buzzer  33  // Set Buzzer pin GPIO 33

#define SW1     39  // Set Switch1 pin GPIO 39
#define SW2     34  // Set Switch2 pin GPIO 34
#define SW3     35  // Set Switch3 pin GPIO 35
#define SW4     27  // Set Switch4 pin GPIO 27

volatile bool relay1State = false;
volatile bool relay2State = false;
volatile bool relay3State = false;
volatile bool relay4State = false;

#define ON  LOW
#define OFF HIGH 

BLYNK_WRITE(V1){
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V1 value is: ");
  Serial.println(pinValue);
  if (pinValue == 0 ) {
    digitalWrite(RLY1, LOW);
    relay1State = pinValue;
  } else {
    digitalWrite(RLY1, HIGH);
    relay1State = pinValue;
  }
  //
}

BLYNK_WRITE(V2){
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V2 value is: ");
  Serial.println(pinValue);
  if (pinValue == 0 ) {
    digitalWrite(RLY2, LOW);
    relay2State = pinValue;
  } else {
    digitalWrite(RLY2, HIGH);
    relay2State = pinValue;
  }
  //
}

BLYNK_WRITE(V3){
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V3 value is: ");
  Serial.println(pinValue);
  if (pinValue == 0 ) {
    digitalWrite(RLY3, LOW);
    relay3State = pinValue;
  } else {
    digitalWrite(RLY3, HIGH);
    relay3State = pinValue;
  }
  //
}

BLYNK_WRITE(V4){
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V4 value is: ");
  Serial.println(pinValue);
  if (pinValue == 0 ) {
    digitalWrite(RLY4, LOW);
    relay4State = pinValue;
  } else {
    digitalWrite(RLY4, HIGH);
    relay4State = pinValue;
  }
  //
}

void setup() {
  Serial.begin(9600);

  pinMode(RLY1, OUTPUT);
  pinMode(RLY2, OUTPUT);
  pinMode(RLY3, OUTPUT);
  pinMode(RLY4, OUTPUT);

  digitalWrite(RLY1, HIGH);
  digitalWrite(RLY2, HIGH);
  digitalWrite(RLY3, HIGH);
  digitalWrite(RLY4, HIGH);
  
  timer1.register_callback(getsensors);

  // wifi
  initWiFi();
 
  // blynk
  blynkBegin();

  // thingsboard
  tb_connect();
  tb.setCallback(callback); // RTC
  // line
  Line_start();

  setupSHT4x();
}

void loop() {
  timer1.run(15000);
  reconnectWiFi();
  Blynk.run();

  // thingsboard
  if (!tb.connected()) {
    tb_reconnect();
  }
  tb.loop();

  getVemlLight();
  delay(1000);
  getSHT4x();
  getsensors();
}

void getsensors(){
  // Create JSON object
  Serial.println("[RUN] getsensors");
  StaticJsonDocument<500> doc;
  doc["luxVal"] = luxVal;
  doc["Blue"] = L_Blue ;
  doc["Red"] = L_Red;
  doc["Green"] = L_Green;
  doc["Clear"] = L_Clear;
  doc["IR"] = L_IR;
  doc["Temperature"] = temperature;
  doc["Humidity"] = humidity;

  // 
  Serial.println("[RUN] Blynk send temperature");
  Serial.println(temperature);
  Blynk.virtualWrite(V5, temperature);

  // Serialize JSON to string
  char buffer[500];
  size_t n = serializeJson(doc, buffer);

  // Publish the JSON payload to ThingsBoard
  Serial.print("Publishing payload: ");
  Serial.println(buffer);

  if (tb.publish("v1/devices/me/telemetry", buffer, n)) {
    Serial.println("Publish acknowledged.");
  } else {
    Serial.println("Publish failed.");
  }
}

// RTC
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("-- [Debug] On message Callback from thingboard");

  char jsonChar[length + 1];
  strncpy(jsonChar, (char *)payload, length);
  jsonChar[length] = '\0';

  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  Serial.println(jsonChar);

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, jsonChar);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  serializeJson(doc, Serial);

  String methodName = doc["method"];
  bool params = doc["params"];

  if (methodName.equals("setRelay1")) {
    digitalWrite(RLY1, params ? LOW : HIGH);
    relay1State = params;
    Blynk.virtualWrite(V1, 0);
  }
  if (methodName.equals("setRelay2")) {
  if (params) {
    digitalWrite(RLY2, LOW);
    Blynk.virtualWrite(V2, 0);
    relay2State = true;
  } else {
    digitalWrite(RLY2, HIGH);
    Blynk.virtualWrite(V2, 1);
    relay2State = false;
  }
}

  if (methodName.equals("setRelay3")) {
    digitalWrite(RLY3, params ? LOW : HIGH);
    relay3State = params;
  }
  if (methodName.equals("setRelay4")) {
    digitalWrite(RLY4, params ? LOW : HIGH);
    relay4State = params;
  }

  // Add get state functionality
  if (methodName.equals("getRelayState")) {
    String responseTopic = String(topic);
    responseTopic.replace("request", "response");
    

    StaticJsonDocument<200> responseDoc;
    responseDoc["relay1State"] = relay1State;
    responseDoc["relay2State"] = relay2State;
    responseDoc["relay3State"] = relay3State;
    responseDoc["relay4State"] = relay4State;

    char responseBuffer[200];
    size_t responseLen = serializeJson(responseDoc, responseBuffer);

    tb.publish(responseTopic.c_str(), responseBuffer, responseLen);
    Serial.print("Response sent: ");
    Serial.println(responseBuffer);
  }
}

// func update status blynk and thingsboard &set status to mem peferance.h 
// how to use peferance.h ประมาณ 3 ไฟล์
// เขียนครอบ func 
// https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/
// มีี service ตรงกลาง แล้วก้มี  func มาอ่าน


