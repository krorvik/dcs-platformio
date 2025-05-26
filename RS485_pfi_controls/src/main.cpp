#define DCSBIOS_RS485_SLAVE 99
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"
// #include "Rotary.h"

// Airpeed indicator
DcsBios::RotaryEncoder airspeedSetKnb("AIRSPEED_SET_KNB", "-3200", "+3200", 3, 4);
// Push function unused, would be pin 9 
// Altimeter
DcsBios::RotaryEncoder altBaroSetKnb("ALT_BARO_SET_KNB", "-3200", "+3200", 5, 6);
// Push function unused, would be pin 10
DcsBios::Switch3Pos altModeLv("ALT_MODE_LV", 11, 12);
// ADI
DcsBios::RotaryEncoder adiPitchTrim("ADI_PITCH_TRIM", "-3200", "+3200", 7, 8);

void setup() {
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}
