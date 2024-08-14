#define DCSBIOS_RS485_SLAVE 43
#define TXENABLE_PIN 2
#include "DcsBios.h"

DcsBios::Switch2Pos slapswitch("CMDS_DISPENSE_BTN", 3);
DcsBios::Switch3Pos canopyswitch("CANOPY_SW", 4, 5);
DcsBios::Switch2Pos spider("CANOPY_HANDLE", 6);
DcsBios::Switch2Pos safeswitch("SEAT_EJECT_SAFE", 7);
DcsBios::Switch2Pos ejecthandle("SEAT_EJECT_HANDLE", 8);

void setup() {
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
