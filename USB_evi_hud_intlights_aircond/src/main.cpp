#define DCSBIOS_IRQ_SERIAL
#include <DcsBios.h>

//Snsr power
DcsBios::Switch2Pos hdptSwL("HDPT_SW_L", 31);
DcsBios::Switch2Pos hdptSwR("HDPT_SW_R", 28);
DcsBios::Switch2Pos fcrPwrSw("FCR_PWR_SW", 29);
DcsBios::Switch3Pos rdrAltPwrSw("RDR_ALT_PWR_SW", 26, 27);

// HUD
DcsBios::Switch3Pos hudScalesSw("HUD_SCALES_SW", 25, 24);
DcsBios::Switch3Pos hudFpMarkerSw("HUD_FP_MARKER_SW", 17, 19);
DcsBios::Switch3Pos hudDedDataSw("HUD_DED_DATA_SW", 16, 14);
DcsBios::Switch3Pos hudDepressRetSw("HUD_DEPRESS_RET_SW", 11, 13);  // 2 ?

DcsBios::Switch3Pos hudSpeedSw("HUD_SPEED_SW", 23, 22);
DcsBios::Switch3Pos hudAltSw("HUD_ALT_SW", 18, 21);
DcsBios::Switch3Pos hudBrtSw("HUD_BRT_SW", 20, 15);
DcsBios::Switch3Pos hudTestSw("HUD_TEST_SW", 12, 10);

// INtlights
DcsBios::Potentiometer priConsolesBrtKnb("PRI_CONSOLES_BRT_KNB", A3);
DcsBios::Potentiometer priInstPnlBrtKnb("PRI_INST_PNL_BRT_KNB", A0);
DcsBios::Potentiometer priDataDisplayBrtKnb("PRI_DATA_DISPLAY_BRT_KNB", A1);

DcsBios::Potentiometer floodConsolesBrtKnb("FLOOD_CONSOLES_BRT_KNB", A2);
DcsBios::Potentiometer floodInstPnlBrtKnb("FLOOD_INST_PNL_BRT_KNB", A5);

// BRT sw 4/5?

// Aircond
DcsBios::Potentiometer tempKnb("TEMP_KNB", A4);

const byte airSourceKnbPins[5] = {6, 7, 8, 9, 3};
DcsBios::SwitchMultiPos airSourceKnb("AIR_SOURCE_KNB", airSourceKnbPins, 5);

void setup() {
  // put your setup code here, to run once:
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}