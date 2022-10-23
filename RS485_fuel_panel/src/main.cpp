#define DCSBIOS_RS485_SLAVE 100
#define TXENABLE_PIN 2
#include "DcsBios.h"

void initalize() {
  for (int i = 3; i <= 9; i++) {
    pinMode(i, INPUT_PULLUP);
  }  
}

DcsBios::Switch2Pos airRefuelSw("AIR_REFUEL_SW", 9);
DcsBios::Switch2Pos fuelMasterSw("FUEL_MASTER_SW", 7);
DcsBios::Switch2Pos tankIntertingSw("TANK_INTERTING_SW", 8);
const byte engineFeedKnbPins[4] = {3, 4, 5, 6};
DcsBios::SwitchMultiPos engineFeedKnb("ENGINE_FEED_KNB", engineFeedKnbPins, 4);

void setup() { 


  
  initalize();
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}
