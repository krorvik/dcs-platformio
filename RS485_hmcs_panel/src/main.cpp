#define DCSBIOS_RS485_SLAVE 91
#define TXENABLE_PIN 2
#include "DcsBios.h"

DcsBios::Potentiometer hmcsIntKnb("HMCS_INT_KNB", A0);

void setup() {
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
