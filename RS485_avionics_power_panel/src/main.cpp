#define DCSBIOS_RS485_SLAVE 97
#define TXENABLE_PIN 2
#include <DcsBios.h>

DcsBios::Switch2Pos dlSw("DL_SW", 8);
DcsBios::Switch2Pos gpsSw("GPS_SW", 9);
DcsBios::Switch2Pos mapSw("MAP_SW", A3);
DcsBios::Switch2Pos mfdSw("MFD_SW", A1);
DcsBios::Switch2Pos mmcPwrSw("MMC_PWR_SW", A4);
DcsBios::Switch2Pos stStaSw("ST_STA_SW", A0);
DcsBios::Switch2Pos ufcSw("UFC_SW", A2);
const byte insKnbPins[7] = {11, A5, 3, 4, 5, 6, 7};
// A3 not in use on PCB
DcsBios::Switch3Pos midsLvtKnb("MIDS_LVT_KNB", 10, 12);

// Moved to another project
/*
DcsBios::Switch3Pos engAntiIce("ENG_ANTI_ICE", 20, 21);
DcsBios::Switch3Pos iffAntSelSw("IFF_ANT_SEL_SW", 22, 23);
DcsBios::Switch3Pos uhfAntSelSw("UHF_ANT_SEL_SW", 24, 25);
*/

// Takes array and array size as arg, not pins
DcsBios::SwitchMultiPos insKnb("INS_KNB", insKnbPins, 7);


void setup() {
  // put your setup code here, to run once:
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}