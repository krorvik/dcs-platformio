#define DCSBIOS_RS485_SLAVE 103
#define TXENABLE_PIN 2
#include "DcsBios.h"
#define PROG_PIN A1
#define MASTER_PIN A0

unsigned int last_anticoll = 0;
unsigned int last_master = 0;

void loop_anticoll() {
  unsigned int new_anticoll = analogRead(PROG_PIN) / 16;
  if (new_anticoll != last_anticoll) {    
    switch (new_anticoll) {
      case 0: 
        sendDcsBiosMessage("ANTI_COLL_LIGHT_KNB", "0"); 
        break;
      case 9:
        sendDcsBiosMessage("ANTI_COLL_LIGHT_KNB", "1");
        break;
      case 18:
        sendDcsBiosMessage("ANTI_COLL_LIGHT_KNB", "2");
        break;
      case 27:
        sendDcsBiosMessage("ANTI_COLL_LIGHT_KNB", "3");
        break;
      case 36:
        sendDcsBiosMessage("ANTI_COLL_LIGHT_KNB", "4");
        break;
      case 45:
        sendDcsBiosMessage("ANTI_COLL_LIGHT_KNB", "5");
        break; 
      case 54:
        sendDcsBiosMessage("ANTI_COLL_LIGHT_KNB", "6");
        break; 
      case 63:
        sendDcsBiosMessage("ANTI_COLL_LIGHT_KNB", "7");
        break; 
    }
   last_anticoll = new_anticoll;
  }
};

void loop_master() {
  unsigned int new_master = analogRead(MASTER_PIN) / 16;
  if (new_master == 31) {
    new_master = 32;
  }
  if (new_master != last_master) {
    switch (new_master) {
      case 0: 
        sendDcsBiosMessage("MASTER_LIGHT_SW", "0"); 
        break;
      case 16:
        sendDcsBiosMessage("MASTER_LIGHT_SW", "1");
        break;
      case 32:
        sendDcsBiosMessage("MASTER_LIGHT_SW", "2");
        break;
      case 48:
        sendDcsBiosMessage("MASTER_LIGHT_SW", "3");
        break;
      case 63:
        sendDcsBiosMessage("MASTER_LIGHT_SW", "4");
        break;
    }
   last_master = new_master;
  }
};


// Static stuff
DcsBios::Potentiometer airRefuelLightKnb("AIR_REFUEL_LIGHT_KNB", A3);
DcsBios::Potentiometer formLightKnb("FORM_LIGHT_KNB", A2);
DcsBios::Switch2Pos posFlashLightSw("POS_FLASH_LIGHT_SW", 5);
DcsBios::Switch3Pos posFuselageLightSw("POS_FUSELAGE_LIGHT_SW", 7,3);
DcsBios::Switch3Pos posWingTailLightSw("POS_WING_TAIL_LIGHT_SW", 6,4);


void setup() {
  DcsBios::setup();
}

void loop() {
  loop_anticoll();
  loop_master();
  DcsBios::loop();
}
