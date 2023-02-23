#define DCSBIOS_RS485_SLAVE 120
#define TXENABLE_PIN 2
#include "DcsBios.h"

// Inputs 4-9. 
DcsBios::Switch2Pos rwrHandoffBtn("RWR_HANDOFF_BTN", 3);
DcsBios::Switch2Pos rwrModeBtn("RWR_MODE_BTN", 4);
DcsBios::Switch2Pos rwrTBtn("RWR_T_BTN", 5);
DcsBios::Switch2Pos rwrLaunchBtn("RWR_LAUNCH_BTN", 6);
DcsBios::Switch2Pos rwrSysTestBtn("RWR_SYS_TEST_BTN", 7);
DcsBios::Switch2Pos rwrUnknownShipBtn("RWR_UNKNOWN_SHIP_BTN", 8);



void setup() {
    DcsBios::setup();
}
void loop() {
    DcsBios::loop();
}