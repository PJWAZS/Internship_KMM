#ifndef MyObjects_h
#define MyObjects_h

Relay R1(RLY1); //Control relay no.1
Relay R2(RLY2); //Control relay no.2
Relay R3(RLY3); //Control relay no.3
Relay R4(RLY4); //Control relay no.4

switchCtl sw1(SW1);
switchCtl sw2(SW2);
switchCtl sw3(SW3);
switchCtl sw4(SW4);

HumiTemp TH01(1);         //Device ID#01
AmbientLight LS01(2);     //Device ID#02

#endif
