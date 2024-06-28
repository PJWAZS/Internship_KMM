#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPL6b4iT-QnP"
#define BLYNK_TEMPLATE_NAME         "Komomi Test"

#include <WiFi.h>
#include <WiFiClient.h>

#include <BlynkSimpleEsp32.h>

WiFiClient espClient;
// char server[] = "203.154.83.151";
char token[] = "JtAY_KnreN8GXyZEXH-x7ZuRXAlH8d1M";
// int port = 8080;

void blynkBegin() {
  // Configure Blynk connection
  Blynk.config(token);
  if (Blynk.connect()) {
    Serial.println("Connected to Blynk server");
  } else {
    Serial.println("Failed to connect to Blynk server");
  }
}

