#include "komomiconnect.hpp"
#include "blynkconnect.hpp"
#include "tbconnect.hpp"
#include "lineconnect.hpp"

void setup() {
  Serial.begin(9600);
  // wifi
  initWiFi();

  // blynk
  blynkBegin();

  // thinsboard
  tb_connect();
  // line
  Line_start();
}

void loop() {
  reconnectWiFi();
  Blynk.run();

  //thinsboard
  if (!tb.connected()) {
    tb_reconnect();
  }
  tb.loop();
}
