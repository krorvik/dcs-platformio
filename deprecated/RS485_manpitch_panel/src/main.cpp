#define DCSBIOS_RS485_SLAVE 92
#define TXENABLE_PIN 2
#include "DcsBios.h"

DcsBios::Switch2Pos epuGenTestSw("MANUAL_PITCH_SW", 3);

void setup() {
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}
