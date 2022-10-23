#define DCSBIOS_RS485_SLAVE 67
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"

#define FIRE_PIN 8
#define OXY_PIN 10
#define MAL_PIN 9
#define PROBE_HEAT_PIN 6
#define PROBE_TEST_PIN 5
#define EPU_PIN 7
#define FLCS_TEST_PIN 3
#define FLCS_MAINT_PIN 4

#define FLCS_A_PIN A0
#define FLCS_B_PIN A1
#define FLCS_C_PIN A2
#define FLCS_D_PIN A3

// DCS Bios hookup
// FLCS power lights
DcsBios::LED lightFlcsPwrA(LIGHT_FLCS_PWR_A_LED_ADDRESS, LIGHT_FLCS_PWR_A_LED_MASK, FLCS_A_PIN);
DcsBios::LED lightFlcsPwrB(LIGHT_FLCS_PWR_B_LED_ADDRESS, LIGHT_FLCS_PWR_B_LED_MASK, FLCS_B_PIN);
DcsBios::LED lightFlcsPwrC(LIGHT_FLCS_PWR_C_LED_ADDRESS, LIGHT_FLCS_PWR_C_LED_MASK, FLCS_C_PIN);
DcsBios::LED lightFlcsPwrD(LIGHT_FLCS_PWR_D_LED_ADDRESS, LIGHT_FLCS_PWR_D_LED_MASK, FLCS_D_PIN);

// Test switches
DcsBios::Switch2Pos epuGenTestSw("EPU_GEN_TEST_SW", EPU_PIN);
DcsBios::Switch3Pos flcsPwrTestSw("FLCS_PWR_TEST_SW", FLCS_TEST_PIN, FLCS_MAINT_PIN);
DcsBios::Switch3Pos probeHeatSw("PROBE_HEAT_SW", PROBE_HEAT_PIN, PROBE_TEST_PIN);
DcsBios::Switch2Pos malIndLtsTest("MAL_IND_LTS_TEST", MAL_PIN);
DcsBios::Switch2Pos obogsSw("OBOGS_SW", OXY_PIN);
DcsBios::Switch2Pos fireOheatDetectBtn("FIRE_OHEAT_DETECT_BTN", FIRE_PIN);

void initalize() {
  for (int i = 3; i<= 10; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  for (int i = A0; i<= A3; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
    delay(1000);
  }
  for (int i = A0; i<= A3; i++) {
    digitalWrite(i, HIGH);   
  }
  delay(1000);
  for (int i = A0; i<= A3; i++) {
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
