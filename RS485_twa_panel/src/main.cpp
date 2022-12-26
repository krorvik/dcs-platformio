#define DCSBIOS_RS485_SLAVE 100
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"


DcsBios::LED lightRwrActPower0(LIGHT_RWR_ACTIVITY_LED_ADDRESS, LIGHT_RWR_ACTIVITY_LED_MASK, 3);
DcsBios::LED lightRwrActPower1(LIGHT_RWR_ACTIVITY_LED_ADDRESS, LIGHT_RWR_ACTIVITY_LED_MASK, 4);

DcsBios::LED lightRwrPower0(LIGHT_RWR_POWER_LED_ADDRESS, LIGHT_RWR_POWER_LED_MASK, 5);
DcsBios::LED lightRwrPower1(LIGHT_RWR_POWER_LED_ADDRESS, LIGHT_RWR_POWER_LED_MASK, 6);

DcsBios::LED lightRwrSearch(LIGHT_RWR_SEARCH_LED_ADDRESS, LIGHT_RWR_SEARCH_LED_MASK, 7);

DcsBios::LED lightRwrAltLow0(LIGHT_RWR_ALT_LOW_LED_ADDRESS, LIGHT_RWR_ALT_LOW_LED_MASK, 8);
DcsBios::LED lightRwrAltLow1(LIGHT_RWR_ALT_LOW_LED_ADDRESS, LIGHT_RWR_ALT_LOW_LED_MASK, 9);

DcsBios::Switch2Pos rwrPwrBtn("RWR_PWR_BTN", A0);
DcsBios::Switch2Pos rwrAltBtn("RWR_ALT_BTN", A1);
DcsBios::Switch2Pos rwrActPwrBtn("RWR_ACT_PWR_BTN", A2);
DcsBios::Switch2Pos rwrSearchBtn("RWR_SEARCH_BTN", A3);

DcsBios::Potentiometer rwrIndDimKnb("RWR_IND_DIM_KNB", A6);
DcsBios::Potentiometer hmcsIntKnb("HMCS_INT_KNB", A7);

void led_init() {
  for (int i = 3; i <= 9; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  delay(2000);
  for (int i = 3; i <= 9; i++) {
    digitalWrite(i, LOW);
  }
}

void setup() {
  led_init();
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
