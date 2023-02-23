
#define DCSBIOS_RS485_SLAVE 71
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"

// Init helper
void initialize() {
  
  for(unsigned int i = 3; i < 14; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
    delay(500);
    digitalWrite(i, LOW);
  }
   
}

//  DcsBios::LED lightRwrHandoffUp(0x447c, 0x0080, PIN); Not on PCB...
DcsBios::LED lightRwrHandoffH(LIGHT_RWR_HANDOFF_H_LED_ADDRESS, LIGHT_RWR_HANDOFF_H_LED_MASK, 3);

DcsBios::LED lightRwrModePri(LIGHT_RWR_MODE_PRI_LED_ADDRESS, LIGHT_RWR_MODE_PRI_LED_MASK, 4);
DcsBios::LED lightRwrModeOpen(LIGHT_RWR_MODE_OPEN_LED_ADDRESS, LIGHT_RWR_MODE_OPEN_LED_MASK, 5);

DcsBios::LED lightRwrTgtsepDn(LIGHT_RWR_TGTSEP_DN_LED_ADDRESS, LIGHT_RWR_TGTSEP_DN_LED_MASK, 6);
DcsBios::LED lightRwrTgtsepUp(LIGHT_RWR_TGTSEP_UP_LED_ADDRESS, LIGHT_RWR_TGTSEP_UP_LED_MASK, 7);

DcsBios::LED lightRwrMslLaunchUp(LIGHT_RWR_MSL_LAUNCH_LED_ADDRESS, LIGHT_RWR_MSL_LAUNCH_LED_MASK, 8);
DcsBios::LED lightRwrMslLaunchDn(LIGHT_RWR_MSL_LAUNCH_LED_ADDRESS, LIGHT_RWR_MSL_LAUNCH_LED_MASK, 9);

DcsBios::LED lightRwrSystestOn(LIGHT_RWR_SYSTEST_LED_ADDRESS, LIGHT_RWR_SYSTEST_LED_MASK, 10);
DcsBios::LED lightRwrSystestDn(LIGHT_RWR_SYSTEST_LED_ADDRESS, LIGHT_RWR_SYSTEST_LED_MASK, 11);

DcsBios::LED lightRwrShipUnkUp(LIGHT_RWR_SHIP_UNK_LED_ADDRESS, LIGHT_RWR_SHIP_UNK_LED_MASK, 12);
DcsBios::LED lightRwrShipUnkDn(LIGHT_RWR_SHIP_UNK_LED_ADDRESS, LIGHT_RWR_SHIP_UNK_LED_MASK, 13);

// A0 - A5 are available, just enough for the buttons...
DcsBios::Switch2Pos rwrHandoffBtn("RWR_HANDOFF_BTN", A1);
DcsBios::Switch2Pos rwrModeBtn("RWR_MODE_BTN", A2);
DcsBios::Switch2Pos rwrTBtn("RWR_T_BTN", A4);
DcsBios::Switch2Pos rwrLaunchBtn("RWR_LAUNCH_BTN", A3);
DcsBios::Switch2Pos rwrSysTestBtn("RWR_SYS_TEST_BTN", A0);
DcsBios::Switch2Pos rwrUnknownShipBtn("RWR_UNKNOWN_SHIP_BTN", A5);

void setup() {
  initialize();  
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}
