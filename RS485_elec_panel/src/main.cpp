
#define DCSBIOS_RS485_SLAVE 70
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"

// Init helper
void initialize() {

  for(unsigned int i = 3; i <= 5; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  
  for(unsigned int i = 6; i <= 12; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  for(unsigned int i = A0; i <= A1; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }

  delay(2000);

  for(unsigned int i = 6; i <= 12; i++) {
    digitalWrite(i, LOW);
  }
  for(unsigned int i = A0; i <= A2; i++) {
    digitalWrite(i, LOW);
  }
  

}

//Switches
DcsBios::Switch2Pos elecCaution("ELEC_CAUTION", 3);
DcsBios::Switch3Pos mainPwrSw("MAIN_PWR_SW", 5, 4);

// LEDs
DcsBios::LED lightFlcsPmg(LIGHT_FLCS_PMG_LED_ADDRESS, LIGHT_FLCS_PMG_LED_MASK, 10);
DcsBios::LED lightMainGen(LIGHT_MAIN_GEN_LED_ADDRESS, LIGHT_MAIN_GEN_LED_MASK, 11);
DcsBios::LED lightStbyGen(LIGHT_STBY_GEN_LED_ADDRESS, LIGHT_STBY_GEN_LED_MASK, 12);
DcsBios::LED lightEpuGen(LIGHT_EPU_GEN_LED_ADDRESS, LIGHT_EPU_GEN_LED_MASK, A1);
DcsBios::LED lightEpuPmg(LIGHT_EPU_PMG_LED_ADDRESS, LIGHT_EPU_PMG_LED_MASK, A0);

DcsBios::LED lightAcftBattFail_UP(LIGHT_ACFT_BATT_FAIL_LED_ADDRESS, LIGHT_ACFT_BATT_FAIL_LED_MASK, 9);
DcsBios::LED lightAcftBattFail_DOWN(LIGHT_ACFT_BATT_FAIL_LED_ADDRESS, LIGHT_ACFT_BATT_FAIL_LED_MASK, 8);

DcsBios::LED lightToFlcs(LIGHT_TO_FLCS_LED_ADDRESS, LIGHT_TO_FLCS_LED_MASK, 7);
DcsBios::LED lightFlcsRly(LIGHT_FLCS_RLY_LED_ADDRESS, LIGHT_FLCS_RLY_LED_MASK, 6);


void setup() {
  initialize();  
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}
