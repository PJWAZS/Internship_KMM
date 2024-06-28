#include <Wire.h>
// sensor enable 31,32
#include <DFRobot_B_LUX_V30B.h>
// DFRobot_B_LUX_V30B myLux(13, 25, 26); //(13, scl, sda) GPIO 22 (SCL clock) and GPIO 21 (SDA)
DFRobot_B_LUX_V30B myLux(13, 22, 21);

void getLight()
{
  Serial.print("\nLight DFRobot");
  Serial.print("value: ");
  Serial.print(myLux.lightStrengthLux());
  Serial.println(" (lux).");
  delay(1000);
}