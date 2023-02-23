#define DCSBIOS_RS485_SLAVE 104
#define TXENABLE_PIN 2
#include "DcsBios.h"

const byte antiCollLightKnbPins[8] = {3, 10, 9, 8, 7, 6, 5, 4};
DcsBios::SwitchMultiPos antiCollLightKnb("ANTI_COLL_LIGHT_KNB", antiCollLightKnbPins, 8);

void setup() {
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
