#ifndef MyTasks_h
#define MyTasks_h

// ***** MULTITASK: ManualSW1 *****
/*
*  MULTITASK
*  ManualSW1: Checking switch status If pressed make relay1 on-off
*
*/
void ManualSW1(void *p) {
 while(true) {
    // On from swich 1 press
    if (sw1.getStatus()==0) {
      if (R1.getStatus()==0) {
        R1.RelayOn();
//        mcp.digitalWrite(pin_led4,HIGH);
//        Serial.println("Relay1 is on");
      }
      else {
        R1.RelayOff();
//        mcp.digitalWrite(pin_led4,LOW);
//        Serial.println("Relay1 is off");
      }
    } 
    //end of main condition     
    vTaskDelay(199 / portTICK_PERIOD_MS);  
 }
}

// ***** MULTITASK: ManualSW2 *****
/*
*  MULTITASK
*  ManualSW2: Checking switch status If pressed make relay2 on-off
*
*/
void ManualSW2(void *p) {
 while(true) {
    // On from swich 2 press
    if (sw2.getStatus()==0) {
      if (R2.getStatus()==0) {
        R2.RelayOn();
//        mcp.digitalWrite(pin_led5,HIGH);
//        Serial.println("Relay2 is on");
      }
      else {
        R2.RelayOff();
//        mcp.digitalWrite(pin_led5,LOW);
//        Serial.println("Relay2 is off");
      }
    } 
    //end of main condition     
    vTaskDelay(223 / portTICK_PERIOD_MS);  
 }
}

// ***** MULTITASK: ManualSW3 *****
/*
*  MULTITASK
*  ManualSW3: Checking switch status If pressed make relay3 on-off
*
*/
void ManualSW3(void *p) {
 while(true) {
    // On from swich 3 press
    if (sw3.getStatus()==0) {
      if (R3.getStatus()==0) {
        R3.RelayOn();
//        mcp.digitalWrite(pin_led6,HIGH);
//        Serial.println("Relay3 is on");
      }
      else {
        R3.RelayOff();
//        mcp.digitalWrite(pin_led6,LOW);
//        Serial.println("Relay3 is off");
      }
    } 
    //end of main condition     
    vTaskDelay(229 / portTICK_PERIOD_MS);  
 }
}

// ***** MULTITASK: ManualSW4 *****
/*
*  MULTITASK
*  ManualSW4: Checking switch status If pressed make relay4 on-off
*
*/
void ManualSW4(void *p) {
 while(true) {
    // On from swich 4 press
    if (sw4.getStatus()==0) {
      if (R4.getStatus()==0) {
        R4.RelayOn();
//        mcp.digitalWrite(pin_led7,HIGH);
//        Serial.println("Relay4 is on");
      }
      else {
        R4.RelayOff();
//        mcp.digitalWrite(pin_led7,LOW);
//        Serial.println("Relay4 is off");
      }
    } 
    //end of main condition     
    vTaskDelay(231 / portTICK_PERIOD_MS);  
 }
}

void Heartbeat(void *p) {
 while(true) {
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
    vTaskDelay(HB_period / portTICK_PERIOD_MS);  
 } //while
}

#endif
