#define DCSBIOS_RS485_SLAVE 74
#define TXENABLE_PIN 2
#include "DcsBios.h"

DcsBios::Switch2Pos voiceinhibitswitch("VMS_INHIBIT_SW", 3);

void setup() {
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}
