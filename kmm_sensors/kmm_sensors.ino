#include "kmm_lightsensors.hpp"
#include "kmm_sht4x.hpp"


void setup() {
  Serial.begin(115200);
  setupSHT4x();

}

void loop() {
  getVemlLight();
  delay(1000);
  getSHT4x();
}
