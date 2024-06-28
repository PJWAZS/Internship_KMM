#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
#define BLYNK_TEMPLATE_NAME         "Device"

#include <WiFi.h>
#include <WiFiClient.h>

#include <BlynkSimpleEsp32.h>

WiFiClient espClient;
char server[] = "203.154.83.151";
char token[] = "9RWfMbs7HIbfhf2ckMSRH7n3hXMQip-i";
int port = 8080;

void blynkBegin() {
  // Configure Blynk connection
  Blynk.config(token, server, port);
  if (Blynk.connect()) {
    Serial.println("Connected to Blynk server");
  } else {
    Serial.println("Failed to connect to Blynk server");
  }
}