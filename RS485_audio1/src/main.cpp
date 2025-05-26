#define DCSBIOS_RS485_SLAVE 82
#define TXENABLE_PIN 2
#include "DcsBios.h"

DcsBios::Switch3Pos comm1ModeKnb("COMM1_MODE_KNB", 4,3);
DcsBios::Switch3Pos comm2ModeKnb("COMM2_MODE_KNB", 6,5);
DcsBios::Switch3Pos hotMicSw("HOT_MIC_SW", 8,7);

DcsBios::Potentiometer comm1PwrKnb("COMM1_PWR_KNB", A0);
DcsBios::Potentiometer comm2PwrKnb("COMM2_PWR_KNB", A1);
DcsBios::Potentiometer secVoiceKnb("SEC_VOICE_KNB", A2);
DcsBios::Potentiometer mslKnb("MSL_KNB", A3);
DcsBios::Potentiometer threatKnb("THREAT_KNB", A4);
DcsBios::Potentiometer intercomKnb("INTERCOM_KNB", A7);
DcsBios::Potentiometer tacanKnb("TACAN_KNB", A6);
DcsBios::Potentiometer ilsPwrKnb("ILS_PWR_KNB", A5);

void setup() {
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}

