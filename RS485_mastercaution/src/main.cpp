// For RS485
#define DCSBIOS_RS485_SLAVE 11
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"

void initalize() {
  for (int i = 3; i < 8; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  delay(2000);
  for (int i = 3; i < 8; i++) {
    digitalWrite(i, LOW);
  }
  for (int i = 9; i<= 12; i++) {
    pinMode(i, INPUT_PULLUP);
  }
}

// Buttons
DcsBios::Switch2Pos masterCaution("MASTER_CAUTION", 9);
DcsBios::Switch2Pos fAckBtn("F_ACK_BTN", 8);
DcsBios::Switch2Pos iffIdBtn("IFF_ID_BTN", 10);
// LEDs
DcsBios::LED lightMasterCaution1(LIGHT_MASTER_CAUTION_LED_ADDRESS, LIGHT_MASTER_CAUTION_LED_MASK, 3);
DcsBios::LED lightMasterCaution2(LIGHT_MASTER_CAUTION_LED_ADDRESS, LIGHT_MASTER_CAUTION_LED_MASK, 4);
DcsBios::LED lightMasterCaution3(LIGHT_MASTER_CAUTION_LED_ADDRESS, LIGHT_MASTER_CAUTION_LED_MASK, 5);
DcsBios::LED lightMasterCaution4(LIGHT_MASTER_CAUTION_LED_ADDRESS, LIGHT_MASTER_CAUTION_LED_MASK, 6);
DcsBios::LED lightTfFail(LIGHT_TF_FAIL_LED_ADDRESS, LIGHT_TF_FAIL_LED_MASK, 7);

void setup() {
  initalize();
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
