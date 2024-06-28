#include <ArduinoJson.h>
#include <Simpletimer.h>
#include <Preferences.h>

#include "komomiconnect.hpp"
#include "blynkconnect.hpp"
#include "tbconnect.hpp"
#include "lineconnect.hpp"
#include "kmm_lightsensors.hpp"
#include "kmm_sht4x.hpp"

Simpletimer timer1{};
Preferences preferences;

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

void saveRelayState() {
  preferences.begin("relay-states", false);
  preferences.putBool("relay1State", relay1State);
  preferences.putBool("relay2State", relay2State);
  preferences.putBool("relay3State", relay3State);
  preferences.putBool("relay4State", relay4State);
  preferences.end();
}

void loadRelayState() {
  preferences.begin("relay-states", true);
  relay1State = preferences.getBool("relay1State", false);
  relay2State = preferences.getBool("relay2State", false);
  relay3State = preferences.getBool("relay3State", false);
  relay4State = preferences.getBool("relay4State", false);
  preferences.end();
}

BLYNK_WRITE(V1){
  int pinValue = param.asInt();
  relay1State = (pinValue == 0);
  digitalWrite(RLY1, relay1State ? LOW : HIGH);
  saveRelayState();
  // Update ThingsBoard status
  StaticJsonDocument<200> doc;
  doc["method"] = "setRelay1";
  doc["params"] = relay1State;
  char buffer[200];
  size_t n = serializeJson(doc, buffer);
  tb.publish("v1/devices/me/rpc/request/+", buffer, n);
}

BLYNK_WRITE(V2){
  int pinValue = param.asInt();
  relay2State = (pinValue == 0);
  digitalWrite(RLY2, relay2State ? LOW : HIGH);
  saveRelayState();
  // Update ThingsBoard status
  StaticJsonDocument<200> doc;
  doc["method"] = "setRelay2";
  doc["params"] = relay2State;
  char buffer[200];
  size_t n = serializeJson(doc, buffer);
  tb.publish("v1/devices/me/rpc/request/+", buffer, n);
}

BLYNK_WRITE(V3){
  int pinValue = param.asInt();
  relay3State = (pinValue == 0);
  digitalWrite(RLY3, relay3State ? LOW : HIGH);
  saveRelayState();
  // Update ThingsBoard status
  StaticJsonDocument<200> doc;
  doc["method"] = "setRelay3";
  doc["params"] = relay3State;
  char buffer[200];
  size_t n = serializeJson(doc, buffer);
  tb.publish("v1/devices/me/rpc/request/+", buffer, n);
}

BLYNK_WRITE(V4){
  int pinValue = param.asInt();
  relay4State = (pinValue == 0);
  digitalWrite(RLY4, relay4State ? LOW : HIGH);
  saveRelayState();
  // Update ThingsBoard status
  StaticJsonDocument<200> doc;
  doc["method"] = "setRelay4";
  doc["params"] = relay4State;
  char buffer[200];
  size_t n = serializeJson(doc, buffer);
  tb.publish("v1/devices/me/rpc/request/+", buffer, n);
}

void setup() {
  Serial.begin(9600);

  pinMode(RLY1, OUTPUT);
  pinMode(RLY2, OUTPUT);
  pinMode(RLY3, OUTPUT);
  pinMode(RLY4, OUTPUT);

  loadRelayState();

  digitalWrite(RLY1, relay1State ? LOW : HIGH);
  digitalWrite(RLY2, relay2State ? LOW : HIGH);
  digitalWrite(RLY3, relay3State ? LOW : HIGH);
  digitalWrite(RLY4, relay4State ? LOW : HIGH);
  
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

  Serial.println("[RUN] Blynk send temperature");
  Serial.println(temperature);
  Blynk.virtualWrite(V5, temperature);

  char buffer[500];
  size_t n = serializeJson(doc, buffer);

  Serial.print("Publishing payload: ");
  Serial.println(buffer);

  if (tb.publish("v1/devices/me/telemetry", buffer, n)) {
    Serial.println("Publish acknowledged.");
  } else {
    Serial.println("Publish failed.");
  }
}

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
    saveRelayState();
    Blynk.virtualWrite(V1, params ? 0 : 1); // Update Blynk status
  }
  if (methodName.equals("setRelay2")) {
    digitalWrite(RLY2, params ? LOW : HIGH);
    relay2State = params;
    saveRelayState();
    Blynk.virtualWrite(V2, params ? 0 : 1); // Update Blynk status
  }
  if (methodName.equals("setRelay3")) {
    digitalWrite(RLY3, params ? LOW : HIGH);
    relay3State = params;
    saveRelayState();
    Blynk.virtualWrite(V3, params ? 0 : 1); // Update Blynk status
  }
  if (methodName.equals("setRelay4")) {
    digitalWrite(RLY4, params ? LOW : HIGH);
    relay4State = params;
    saveRelayState();
    Blynk.virtualWrite(V4, params ? 0 : 1); // Update Blynk status
  }

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


