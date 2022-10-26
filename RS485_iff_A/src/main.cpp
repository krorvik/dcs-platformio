#define DCSBIOS_RS485_SLAVE 106
#define TXENABLE_PIN 2
#include "DcsBios.h"

const byte iffMasterKnbPins[5] = {8, 7, 6, 5, 4};
DcsBios::SwitchMultiPos iffMasterKnb("IFF_MASTER_KNB", iffMasterKnbPins, 5);

DcsBios::Switch2Pos iffCIKnb("IFF_C_I_KNB", 3);
DcsBios::Switch3Pos iffEnableSw("IFF_ENABLE_SW", 11, 12);
DcsBios::Switch3Pos iffM4CodeSw("IFF_M4_CODE_SW", 9, 10);
DcsBios::Switch2Pos iffM4MonitorSw("IFF_M4_MONITOR_SW", A2);
DcsBios::Switch3Pos iffM4ReplySw("IFF_M4_REPLY_SW", A1, A0);

void setup() {
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
