#include <WiFi.h>      // Replace with WiFi.h for ESP32

const char* ssid = "MONIC";
const char* password = "1000603413";

unsigned long previousMillis = 0; // Stores the last time WiFi status was checked
const long interval = 10000; // Interval at which to check WiFi status (1 second)


// Function to initialize WiFi connection
void initWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  // Wait until the connection has been confirmed before continuing
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Print RSSI (signal strength)
  long rssi = WiFi.RSSI();
  Serial.print("RSSI: ");
  Serial.println(rssi);

  // Print MAC address of the router
  String macAddress = WiFi.BSSIDstr();
  Serial.print("MAC address of the router: ");
  Serial.println(macAddress);
}

// Function to reconnect WiFi if connection is lost
void reconnectWiFi() {
  unsigned long currentMillis = millis();

  // Check WiFi connection status every interval milliseconds
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi connection lost. Reconnecting...");
      initWiFi();
    }
  }
}


