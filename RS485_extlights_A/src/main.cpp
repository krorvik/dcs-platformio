#define DCSBIOS_RS485_SLAVE 103
#define TXENABLE_PIN 2
#include "DcsBios.h"

DcsBios::Potentiometer airRefuelLightKnb("AIR_REFUEL_LIGHT_KNB", A7);
DcsBios::Potentiometer formLightKnb("FORM_LIGHT_KNB", A6);

const byte masterLightSwPins[5] = {3, 4, 5, 6, 7};
DcsBios::SwitchMultiPos masterLightSw("MASTER_LIGHT_SW", masterLightSwPins, 5);

DcsBios::Switch2Pos posFlashLightSw("POS_FLASH_LIGHT_SW", 10);
DcsBios::Switch3Pos posFuselageLightSw("POS_FUSELAGE_LIGHT_SW", 9, 8);
DcsBios::Switch3Pos posWingTailLightSw("POS_WING_TAIL_LIGHT_SW", 11, 12);


void setup() {
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
