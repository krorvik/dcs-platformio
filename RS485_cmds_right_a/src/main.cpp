#define DCSBIOS_RS485_SLAVE 69
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"

// Toggle switches
DcsBios::Switch2Pos cmds01ExpCatSw("CMDS_01_EXP_CAT_SW", 6);
DcsBios::Switch2Pos cmds02ExpCatSw("CMDS_02_EXP_CAT_SW", 5);
DcsBios::Switch2Pos cmdsChExpCatSw("CMDS_CH_EXP_CAT_SW", 4);
DcsBios::Switch2Pos cmdsFlExpCatSw("CMDS_FL_EXP_CAT_SW", 3);
DcsBios::Switch2Pos cmdsJettSw("CMDS_JETT_SW", 12);
DcsBios::Switch2Pos cmdsMwsSourcheSw("CMDS_MWS_SOURCE_SW", 9);
DcsBios::Switch2Pos cmdsJmrSourcheSw("CMDS_JMR_SOURCE_SW", 8);
DcsBios::Switch2Pos cmdsPwrSourcheSw("CMDS_RWR_SOURCE_SW", 7);

DcsBios::LED lightCmdsNoGo1(LIGHT_CMDS_NO_GO_LED_ADDRESS, LIGHT_CMDS_NO_GO_LED_ADDRESS, A0);
DcsBios::LED lightCmdsNoGo0(LIGHT_CMDS_NO_GO_LED_ADDRESS, LIGHT_CMDS_NO_GO_LED_ADDRESS, A1);
DcsBios::LED lightCmdsGo(LIGHT_CMDS_GO_LED_ADDRESS, LIGHT_CMDS_GO_LED_MASK, A2);
DcsBios::LED lightCmdsDisp0(LIGHT_CMDS_DISP_LED_ADDRESS, LIGHT_CMDS_DISP_LED_MASK, A3);
DcsBios::LED lightCmdsDisp1(LIGHT_CMDS_DISP_LED_ADDRESS, LIGHT_CMDS_DISP_LED_MASK, A4);
DcsBios::LED lightCmdsRdy(LIGHT_CMDS_RDY_LED_ADDRESS, LIGHT_CMDS_RDY_LED_MASK, A5);

// Init helper
void initialize() {
  for(unsigned int i = A0; i <= A5; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
    delay(1000);
    digitalWrite(i, LOW);
  }
 
}


void setup() {
  initialize();  
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
  
}
