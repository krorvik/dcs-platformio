#define DCSBIOS_RS485_SLAVE 114
#define TXENABLE_PIN 2
#include "DcsBios.h"


DcsBios::Switch2Pos advModeSw("ADV_MODE_SW", 3);
DcsBios::Switch3Pos masterArmSw("MASTER_ARM_SW", A1, A0);
DcsBios::Switch2Pos laserArmSw("LASER_ARM_SW", A2);
DcsBios::Switch2Pos altRelBtn("ALT_REL_BTN", A3);
DcsBios::Switch3Pos apPitchSw("AP_PITCH_SW", 4, 5);
DcsBios::Switch3Pos apRollSw("AP_ROLL_SW", 6, 7);
DcsBios::Switch3Pos rfSw("RF_SW", 8, 9);

void setup() {
    DcsBios::setup();
}

void loop() {
    DcsBios::loop();
}