
#define RXD0 3
#define TXD0 1

#define RXD1 26
#define TXD1 25

#define RXD2 16
#define TXD2 17


#define RELAY1 17
#define RELAY2 16
#define RELAY3 14
#define RELAY4 15

#define MODEM_POWER_ON 4
#define MODEM_KEY_ON 13
#define MODEM_STATUS 36

#define BUZZER 33

#define RELAY1_ON digitalWrite(RELAY1, LOW)
#define RELAY2_ON digitalWrite(RELAY2, LOW)
#define RELAY3_ON digitalWrite(RELAY3, LOW)
#define RELAY4_ON digitalWrite(RELAY4, LOW)

#define RELAY1_OFF digitalWrite(RELAY1, HIGH)
#define RELAY2_OFF digitalWrite(RELAY2, HIGH)
#define RELAY3_OFF digitalWrite(RELAY3, HIGH)
#define RELAY4_OFF digitalWrite(RELAY4, HIGH)

void setupGpio() {
  pinMode(RELAY1, OUTPUT);
  RELAY1_OFF;
  pinMode(RELAY2, OUTPUT);
  RELAY2_OFF;
  pinMode(RELAY3, OUTPUT);
  RELAY3_OFF;
  pinMode(RELAY4, OUTPUT);
  RELAY4_OFF;
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, HIGH);
  RELAY1_OFF;
  RELAY2_OFF;
  RELAY3_OFF;
  RELAY4_OFF;

  pinMode(MODEM_POWER_ON, OUTPUT);
  pinMode(MODEM_KEY_ON, OUTPUT);
  pinMode(MODEM_STATUS, INPUT);


}

void testGPIO()
{
  Serial.println("[Test] GPIO");
  delay(2000);
  RELAY1_ON;
  delay(2000);
  RELAY1_OFF;

  RELAY2_ON;
  delay(2000);
  RELAY2_OFF;

  RELAY3_ON;
  delay(2000);
  RELAY3_OFF;

  RELAY4_ON;
  delay(2000);
  RELAY4_OFF;


}


void oneBeeb()
{
  digitalWrite(BUZZER, LOW);
  delay(1000);
  digitalWrite(BUZZER, HIGH);
  delay(1000);
}

void twoBeep()
{
  oneBeeb();
  oneBeeb();
}

void threeBeep()
{
  oneBeeb();
  oneBeeb();
  oneBeeb();
}
