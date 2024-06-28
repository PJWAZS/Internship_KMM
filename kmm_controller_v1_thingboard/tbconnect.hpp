#define TOKEN               "SCxHRsU4zUr5OY2dWzsQ"   //Mainbuilding
#define DEVICE_ID           "6feaa260-22f3-11ef-a435-ab3a1d535f3e"
#define THINGSBOARD_SERVER  "demo.thingsboard.io"   //komomidev+royal@gmail.com

#include <WiFi.h>  
#include <PubSubClient.h>

WiFiClient espClient2;
PubSubClient tb(espClient2);

void tb_connect() {
    tb.setServer(THINGSBOARD_SERVER, 1883);

    if (!tb.connect(DEVICE_ID, TOKEN, "")) {
      Serial.println("Failed to connect to thingsboard server");
      return;
    } else {
      Serial.println("Success to connect to thingsboard server");
    }
    tb.subscribe("v1/devices/me/rpc/request/+"); // RTC
}


void tb_reconnect() {
  // Loop until we're reconnected
  int try_num = 0;
  while (!tb.connected() && try_num < 5) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (tb.connect(DEVICE_ID, TOKEN, "")) {
      Serial.println("connected");
      tb.subscribe("v1/devices/me/rpc/request/+"); // RTC
      break;
    } else {
      Serial.print("failed, rc=");
      Serial.print(tb.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    try_num++;
  }
}

void tb_loob() {
  tb.loop();
}


