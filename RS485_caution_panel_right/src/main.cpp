#define DCSBIOS_RS485_SLAVE 8
#define TXENABLE_PIN 2
#include <DcsBios.h>
#include "f16c_data.h"

unsigned int pins[16] = { 3,4,5,6,7,8,9,10,11,12,13,A0,A1,A2,A3,A4};

DcsBios::LED LIGHT_AVIONICS_FAULT ( LIGHT_AVIONICS_FAULT_LED_ADDRESS ,  LIGHT_AVIONICS_FAULT_LED_MASK , pins[0]);
DcsBios::LED LIGHT_EQUIP_HOT ( LIGHT_EQUIP_HOT_LED_ADDRESS ,  LIGHT_EQUIP_HOT_LED_MASK , pins[1]);
DcsBios::LED LIGHT_RADAR_ALT ( LIGHT_RADAR_ALT_LED_ADDRESS ,  LIGHT_RADAR_ALT_LED_MASK , pins[2]);
DcsBios::LED LIGHT_IFF ( LIGHT_IFF_LED_ADDRESS ,  LIGHT_IFF_LED_MASK , pins[3]);
DcsBios::LED LIGHT_NUCLEAR ( LIGHT_NUCLEAR_LED_ADDRESS ,  LIGHT_NUCLEAR_LED_MASK , pins[4]);
DcsBios::LED LIGHT_CAUTION_2 ( LIGHT_CAUTION_2_LED_ADDRESS ,  LIGHT_CAUTION_2_LED_MASK , pins[5]);
DcsBios::LED LIGHT_CAUTION_3 ( LIGHT_CAUTION_3_LED_ADDRESS ,  LIGHT_CAUTION_3_LED_MASK , pins[6]);
DcsBios::LED LIGHT_CAUTION_4 ( LIGHT_CAUTION_4_LED_ADDRESS ,  LIGHT_CAUTION_4_LED_MASK , pins[7]);

DcsBios::LED LIGHT_SEAT_NOT ( LIGHT_SEAT_NOT_LED_ADDRESS ,  LIGHT_SEAT_NOT_LED_MASK , pins[8]);
DcsBios::LED LIGHT_NWS_FAIL ( LIGHT_NWS_FAIL_LED_ADDRESS ,  LIGHT_NWS_FAIL_LED_MASK , pins[9]);
DcsBios::LED LIGHT_ANTI_SKID ( LIGHT_ANTI_SKID_LED_ADDRESS ,  LIGHT_ANTI_SKID_LED_MASK , pins[10]);
DcsBios::LED LIGHT_HOOK ( LIGHT_HOOK_LED_ADDRESS ,  LIGHT_HOOK_LED_MASK , pins[11]);
DcsBios::LED LIGHT_OBOGS ( LIGHT_OBOGS_LED_ADDRESS ,  LIGHT_OBOGS_LED_MASK , pins [12]);
DcsBios::LED LIGHT_CABIN_PRESS ( LIGHT_CABIN_PRESS_LED_ADDRESS ,  LIGHT_CABIN_PRESS_LED_MASK , pins[13]);
DcsBios::LED LIGHT_CAUTION_5 ( LIGHT_CAUTION_5_LED_ADDRESS ,  LIGHT_CAUTION_5_LED_MASK , pins[14]);
DcsBios::LED LIGHT_CAUTION_6 ( LIGHT_CAUTION_6_LED_ADDRESS ,  LIGHT_CAUTION_6_LED_MASK , pins[15]);

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

