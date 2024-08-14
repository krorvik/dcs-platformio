#define DCSBIOS_RS485_SLAVE 7
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"

const int eyebrowpins[8] = { 3, 4, 5, 6, 7, 8, 9, 10};

DcsBios::LED LIGHT_ENGINE ( LIGHT_ENGINE_LED_ADDRESS ,  LIGHT_ENGINE_LED_MASK , eyebrowpins[0]);
DcsBios::LED LIGHT_ENG_FIRE ( LIGHT_ENG_FIRE_LED_ADDRESS ,  LIGHT_ENG_FIRE_LED_MASK , eyebrowpins[1]);
DcsBios::LED LIGHT_HYD_OIL_PRESS ( LIGHT_HYD_OIL_PRESS_LED_ADDRESS ,  LIGHT_HYD_OIL_PRESS_LED_MASK , eyebrowpins[2]);
DcsBios::LED LIGHT_DBU_ON ( LIGHT_DBU_ON_LED_ADDRESS ,  LIGHT_DBU_ON_LED_MASK , eyebrowpins[3]);
DcsBios::LED LIGHT_FLCS ( LIGHT_FLCS_LED_ADDRESS ,  LIGHT_FLCS_LED_MASK , eyebrowpins[4]);
DcsBios::LED LIGHT_TO_LDG_CONFIG ( LIGHT_TO_LDG_CONFIG_LED_ADDRESS ,  LIGHT_TO_LDG_CONFIG_LED_MASK , eyebrowpins[5]);
DcsBios::LED LIGHT_OXY_LOW ( LIGHT_OXY_LOW_LED_ADDRESS ,  LIGHT_OXY_LOW_LED_MASK , eyebrowpins[6]);
DcsBios::LED LIGHT_CANOPY ( LIGHT_CANOPY_LED_ADDRESS ,  LIGHT_CANOPY_LED_MASK , eyebrowpins[7]);

void setup() {

  for (int index = 0; index < 8; index++) {
    pinMode(eyebrowpins[index], OUTPUT);
    digitalWrite(eyebrowpins[index], HIGH);
  }

  delay(2000);
  // Eyebrow Lights off
  for (int index = 0; index < 8; index++) {
    digitalWrite(eyebrowpins[index], LOW);
  }
  DcsBios::setup();
}

void loop() {
  // And DCS bios loop.
  DcsBios::loop();
}
