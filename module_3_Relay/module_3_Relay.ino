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

void IRAM_ATTR handleSW1() {
  relay1State = !relay1State;  // Toggle relay state
  Serial.println("[SW1] digitalRead");
  Serial.println(digitalRead(SW1));
  digitalWrite(RLY1, relay1State ? HIGH : LOW);
  digitalWrite(Buzzer, relay1State ? HIGH : LOW);  // Example: control buzzer based on SW1
}

void IRAM_ATTR handleSW2() {
  Serial.println("[SW2] digitalRead");
  Serial.println(digitalRead(SW2));
  relay2State = !relay2State;  // Toggle relay state
  digitalWrite(RLY2, relay2State ? HIGH : LOW);
}

void IRAM_ATTR handleSW3() {
  Serial.println("[SW3] digitalRead");
  Serial.println(digitalRead(SW3));
  relay3State = !relay3State;  // Toggle relay state
  digitalWrite(RLY3, relay3State ? HIGH : LOW);
}

void IRAM_ATTR handleSW4() {
  Serial.println("[SW4] digitalRead");
  Serial.println(digitalRead(SW4));
  relay4State = !relay4State;  // Toggle relay state
  digitalWrite(RLY4, relay4State ? HIGH : LOW);
}

void setup() {
  Serial.begin(9600);
  // Initialize relay pins as outputs
  pinMode(RLY1, OUTPUT);
  pinMode(RLY2, OUTPUT);
  pinMode(RLY3, OUTPUT);
  pinMode(RLY4, OUTPUT);
  
  // Initialize the buzzer pin as output
  pinMode(Buzzer, OUTPUT);
  
  // Initialize switch pins as inputs
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(SW4, INPUT);

  // Start with all relays and buzzer off
  digitalWrite(RLY1, LOW);
  digitalWrite(RLY2, LOW);
  digitalWrite(RLY3, LOW);
  digitalWrite(RLY4, LOW);
  digitalWrite(Buzzer, LOW);

  // Attach interrupts to the switch pins
  attachInterrupt(digitalPinToInterrupt(SW1), handleSW1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(SW2), handleSW2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(SW3), handleSW3, CHANGE);
  attachInterrupt(digitalPinToInterrupt(SW4), handleSW4, CHANGE);
}

void loop() {
  // Read the state of each switch
  int sw1State = digitalRead(SW1);
  int sw2State = digitalRead(SW2);
  int sw3State = digitalRead(SW3);
  int sw4State = digitalRead(SW4);

  // Control relays based on switch states
  if (sw1State == HIGH) {
    digitalWrite(RLY1, HIGH);  // Turn Relay1 on
  } else {
    digitalWrite(RLY1, LOW);   // Turn Relay1 off
  }
  
  if (sw2State == HIGH) {
    digitalWrite(RLY2, HIGH);  // Turn Relay2 on
  } else {
    digitalWrite(RLY2, LOW);   // Turn Relay2 off
  }
  
  if (sw3State == HIGH) {
    digitalWrite(RLY3, HIGH);  // Turn Relay3 on
  } else {
    digitalWrite(RLY3, LOW);   // Turn Relay3 off
  }
  
  if (sw4State == HIGH) {
    digitalWrite(RLY4, HIGH);  // Turn Relay4 on
  } else {
    digitalWrite(RLY4, LOW);   // Turn Relay4 off
  }

  // Example: control buzzer based on a specific switch (e.g., SW1)
  if (sw1State == HIGH) {
    digitalWrite(Buzzer, HIGH);  // Turn Buzzer on
  } else {
    digitalWrite(Buzzer, LOW);   // Turn Buzzer off
  }

  // Small delay to debounce switches
  delay(50);
}
