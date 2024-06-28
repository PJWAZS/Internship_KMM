#include "BluetoothSerial.h"  // Include the Bluetooth library

BluetoothSerial SerialBT;

const int ledPin = 2; // Pin to which the LED is connected

void setup() {
  pinMode(ledPin, OUTPUT); // Set the LED pin as output
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT"); // Set the Bluetooth device name
  Serial.println("The device started, now you can pair it with Bluetooth!");
}

void loop() {
  if (SerialBT.available()) {
    char incomingChar = SerialBT.read(); // Read the incoming data
    if (incomingChar == '1') {
      digitalWrite(ledPin, HIGH); // Turn on the LED
    } else if (incomingChar == '0') {
      digitalWrite(ledPin, LOW); // Turn off the LED
    }
    Serial.print("Received: ");
    Serial.println(incomingChar);
  }
}
