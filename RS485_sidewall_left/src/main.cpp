#define DCSBIOS_RS485_SLAVE 45
#define TXENABLE_PIN 2
#include "DcsBios.h"

// 3 - Lock
// 4 - canopy
// 5 - canopy
// 6 - CMDS panic

DcsBios::Switch2Pos canopyHandle("CANOPY_HANDLE", 3);
DcsBios::Switch3Pos canopySw("CANOPY_SW", 4, 5);
DcsBios::Switch2Pos cmdsDispenseBtn("CMDS_DISPENSE_BTN", 6);

void setup() {
    DcsBios::setup();
}

void loop() {
    DcsBios::loop();
}