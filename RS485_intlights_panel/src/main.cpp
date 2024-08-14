#define DCSBIOS_RS485_SLAVE 96
#define TXENABLE_PIN 2
#include <DcsBios.h>

DcsBios::Potentiometer priConsolesBrtKnb("PRI_CONSOLES_BRT_KNB", A2);
DcsBios::Potentiometer priInstPnlBrtKnb("PRI_INST_PNL_BRT_KNB", A3);
DcsBios::Potentiometer priDataDisplayBrtKnb("PRI_DATA_DISPLAY_BRT_KNB", A4);

DcsBios::Potentiometer floodConsolesBrtKnb("FLOOD_CONSOLES_BRT_KNB", A1);
DcsBios::Potentiometer floodInstPnlBrtKnb("FLOOD_INST_PNL_BRT_KNB", A0);

DcsBios::Potentiometer tempKnb("TEMP_KNB", A5);

const byte airSourceKnbPins[5] = {3, 4, 5, 6, 12}; // Need fix, expect too many pins, one must be assigned to unused pin

DcsBios::Switch3Pos malIndBrtSw("MAL_IND_LTS_BRT_SW", 7,8); // huh?
DcsBios::SwitchMultiPos airSourceKnb("AIR_SOURCE_KNB", airSourceKnbPins, 5);


void setup() {
  // put your setup code here, to run once:
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}