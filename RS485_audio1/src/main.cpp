#define DCSBIOS_RS485_SLAVE 82
#define TXENABLE_PIN 2
#include "DcsBios.h"

void initalize() {
  for (int i = 3; i <= 6; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  for (int i = A0; i <= A7; i++) {
    pinMode(i, INPUT);
  }
}

DcsBios::Switch3Pos comm1ModeKnb("COMM1_MODE_KNB", 5, 6);
DcsBios::Switch3Pos comm2ModeKnb("COMM2_MODE_KNB", 3, 4);

DcsBios::Potentiometer comm1PwrKnb("COMM1_PWR_KNB", A0);
DcsBios::Potentiometer comm2PwrKnb("COMM2_PWR_KNB", A1);
DcsBios::Potentiometer secVoiceKnb("SEC_VOICE_KNB", A2);
DcsBios::Potentiometer mslKnb("MSL_KNB", A3);
DcsBios::Potentiometer tfKnb("TF_KNB", A7);
DcsBios::Potentiometer threatKnb("THREAT_KNB", A6);

void setup() {
  initalize();
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}

