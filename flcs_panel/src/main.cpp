
#define DCSBIOS_RS485_SLAVE 68
#define TXENABLE_PIN 2
#include "DcsBios.h"

#define DBU_PIN 4
#define ALTFLAPS_PIN 5
#define MANUAL_FLYUP_PIN 3
#define LEFLAPSLOCK_PIN 6
#define FLCSRESET_PIN 7
#define FLCS_BIT_PIN 8

#define FLCS_FAIL_PIN A0
#define FLCS_RUN_PIN A1

// DCS Bios hookup
// FLCS BIT lights
DcsBios::LED lightFlFail(0x447a, 0x1000, FLCS_FAIL_PIN);
DcsBios::LED lightFlRun(0x447a, 0x0800, FLCS_RUN_PIN);

// FLCS switches
DcsBios::Switch2Pos bitSw("BIT_SW", FLCS_BIT_PIN);
DcsBios::Switch2Pos altFlapsSw("ALT_FLAPS_SW", ALTFLAPS_PIN);
DcsBios::Switch2Pos digiBakSw("DIGI_BAK_SW", DBU_PIN);
DcsBios::Switch2Pos flcsResetSw("FLCS_RESET_SW", FLCSRESET_PIN);
DcsBios::Switch2Pos leFlapsSw("LE_FLAPS_SW", LEFLAPSLOCK_PIN);
DcsBios::Switch2Pos manTfFlyupSw("MAN_TF_FLYUP_SW", MANUAL_FLYUP_PIN);



void initalize() {
  for (int i = 3; i<= 10; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  for (int i = A0; i<= A1; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
    delay(1000);
    digitalWrite(i, LOW);
  }
  for (int i = A0; i<= A1; i++) {
    digitalWrite(i, HIGH);   
  }
  delay(1000);
  for (int i = A0; i<= A1; i++) {
    digitalWrite(i, LOW);   
  }
}

void setup() {
  initalize();
  DcsBios::setup();
}



void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}