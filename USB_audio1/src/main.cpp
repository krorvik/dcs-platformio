#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"

// DcsBios::Switch3Pos comm1ModeKnb("COMM1_MODE_KNB", 5, 6);
// DcsBios::Switch3Pos comm2ModeKnb("COMM2_MODE_KNB", 3, 4);

// Audio 1 
// Pots
DcsBios::Potentiometer comm1PwrKnb("COMM1_PWR_KNB", A0);
DcsBios::Potentiometer comm2PwrKnb("COMM2_PWR_KNB", A1);
DcsBios::Potentiometer secVoiceKnb("SEC_VOICE_KNB", A2);
DcsBios::Potentiometer mslKnb("MSL_KNB", A3);
DcsBios::Potentiometer threatKnb("THREAT_KNB", A4);
DcsBios::Potentiometer tfKnb("TF_KNB", A5);
// Switches
DcsBios::Switch3Pos comm1ModeKnb("COMM1_MODE_KNB", 4, 5);
DcsBios::Switch3Pos comm2ModeKnb("COMM2_MODE_KNB", 6, 7);

// Audio 2 
// Pots
DcsBios::Potentiometer intercomKnb("INTERCOM_KNB", A6);
DcsBios::Potentiometer tacanKnb("TACAN_KNB", A7);
DcsBios::Potentiometer ilsPwrKnb("ILS_PWR_KNB", A8);
// Switch
DcsBios::Switch3Pos hotMicSw("HOT_MIC_SW", 9,8);

/*
// Trim rudder pot
DcsBios::Potentiometer yawTrim("YAW_TRIM", A9);

// extlights
DcsBios::Potentiometer airRefuelLightKnb("AIR_REFUEL_LIGHT_KNB", A10);
DcsBios::Potentiometer formLightKnb("FORM_LIGHT_KNB", A11);

// UHF volume
DcsBios::Potentiometer uhfVolKnb("UHF_VOL_KNB", A12);
// HMCS intensity
DcsBios::Potentiometer hmcsIntKnb("HMCS_INT_KNB", A13);

// TWA intensity
DcsBios::Potentiometer rwrIndDimKnb("RWR_IND_DIM_KNB", A14);
// ECM intensity
DcsBios::Potentiometer ecmDimKnb("ECM_DIM_KNB", A15); 
*/

void setup() {
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}
