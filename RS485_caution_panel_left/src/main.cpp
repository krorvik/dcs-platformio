#define DCSBIOS_RS485_SLAVE 9
#define TXENABLE_PIN 2
#include <DcsBios.h>
#include "f16c_data.h"

unsigned int pins[16] = { 3,4,5,6,7,8,9,10,11,12,13,A0,A1,A2,A3,A4};

DcsBios::LED LIGHT_FLCS_FAULT ( LIGHT_FLCS_FAULT_LED_ADDRESS ,  LIGHT_FLCS_FAULT_LED_MASK , pins[0]);
DcsBios::LED LIGHT_ELEC_SYS ( LIGHT_ELEC_SYS_LED_ADDRESS ,  LIGHT_ELEC_SYS_LED_MASK , pins[1]);
DcsBios::LED LIGHT_PROBE_HEAT ( LIGHT_PROBE_HEAT_LED_ADDRESS ,  LIGHT_PROBE_HEAT_LED_MASK , pins[2]);
DcsBios::LED LIGHT_CADC ( LIGHT_CADC_LED_ADDRESS ,  LIGHT_CADC_LED_MASK , pins[3]);
DcsBios::LED LIGHT_STORES_CONFIG ( LIGHT_STORES_CONFIG_LED_ADDRESS ,  LIGHT_STORES_CONFIG_LED_MASK , pins[4]);
DcsBios::LED LIGHT_ATF_NOT ( LIGHT_ATF_NOT_LED_ADDRESS ,  LIGHT_ATF_NOT_LED_MASK , pins[5]);
DcsBios::LED LIGHT_FWD_FUEL_LOW ( LIGHT_FWD_FUEL_LOW_LED_ADDRESS ,  LIGHT_FWD_FUEL_LOW_LED_MASK , pins[6]);
DcsBios::LED LIGHT_AFT_FUEL_LOW ( LIGHT_AFT_FUEL_LOW_LED_ADDRESS ,  LIGHT_AFT_FUEL_LOW_LED_MASK , pins[7]);

DcsBios::LED LIGHT_ENGINE_FAULT ( LIGHT_ENGINE_FAULT_LED_ADDRESS ,  LIGHT_ENGINE_FAULT_LED_MASK , pins[8]);
DcsBios::LED LIGHT_SEC ( LIGHT_SEC_LED_ADDRESS ,  LIGHT_SEC_LED_MASK , pins[9]);
DcsBios::LED LIGHT_FUEL_OIL_HOT ( LIGHT_FUEL_OIL_HOT_LED_ADDRESS ,  LIGHT_FUEL_OIL_HOT_LED_MASK , pins[10]);
DcsBios::LED LIGHT_INLET_ICING ( LIGHT_INLET_ICING_LED_ADDRESS ,  LIGHT_INLET_ICING_LED_MASK , pins[11]);
DcsBios::LED LIGHT_OVERHEAT ( LIGHT_OVERHEAT_LED_ADDRESS ,  LIGHT_OVERHEAT_LED_MASK , pins[12]);
DcsBios::LED LIGHT_EEC ( LIGHT_EEC_LED_ADDRESS ,  LIGHT_EEC_LED_MASK , pins[13]);
DcsBios::LED LIGHT_BUC ( LIGHT_BUC_LED_ADDRESS ,  LIGHT_BUC_LED_MASK , pins[14]);
DcsBios::LED LIGHT_CAUTION_1 ( LIGHT_CAUTION_1_LED_ADDRESS ,  LIGHT_CAUTION_1_LED_MASK , pins[15]);

void initialize_pins() {

  for (int i = 0; i < 16; i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], HIGH);
    delay(200);
    digitalWrite(pins[i], LOW);
  }
  for (int i = 0; i < 16; i++) {
    digitalWrite(pins[i], HIGH);
  }
  delay(1000);
  for (int i = 0; i < 16; i++) {
    digitalWrite(pins[i], LOW);
  }
}

void setup() {
  initialize_pins();
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}

