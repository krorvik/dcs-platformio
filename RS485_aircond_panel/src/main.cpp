#define DCSBIOS_RS485_SLAVE 93
#define TXENABLE_PIN 2
#include <DcsBios.h>

DcsBios::Potentiometer tempKnb("TEMP_KNB", A7);
const byte airSourceKnbPins[5] = {3, 4, 5, 6, 7};

DcsBios::Switch2Pos vmsInhibitSw("VMS_INHIBIT_SW", 12);

DcsBios::SwitchMultiPos airSourceKnb("AIR_SOURCE_KNB", airSourceKnbPins, 5);


void setup() {
  // put your setup code here, to run once:
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}