#define DCSBIOS_RS485_SLAVE 95
#define TXENABLE_PIN 2
#include <DcsBios.h>

DcsBios::Switch3Pos hudScalesSw("HUD_SCALES_SW", 12, 11);
DcsBios::Switch3Pos hudFpMarkerSw("HUD_FP_MARKER_SW", 18, 14);
DcsBios::Switch3Pos hudDedDataSw("HUD_DED_DATA_SW", 19, 5);
DcsBios::Switch3Pos hudDepressRetSw("HUD_DEPRESS_RET_SW", 7, 8);

DcsBios::Switch3Pos hudSpeedSw("HUD_SPEED_SW", 16, 17);
DcsBios::Switch3Pos hudAltSw("HUD_ALT_SW", 15, 3);
DcsBios::Switch3Pos hudBrtSw("HUD_BRT_SW", 4, 6);
DcsBios::Switch3Pos hudTestSw("HUD_TEST_SW", 10, 9);

void setup() {
  // put your setup code here, to run once:
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}