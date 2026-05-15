#define DCSBIOS_RS485_SLAVE 99
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"
#include "Button2.h"

#define ALTMODEPIN A3
Button2 altmodelever;
unsigned altmode_current = 0;

// Encoders
// Set forward adjustment greater, can dial back on "too far"
DcsBios::RotaryEncoder airspeedSetKnb("AIRSPEED_SET_KNB", "-800", "+1600", 6, 5);
DcsBios::RotaryEncoder altBaroSetKnb("ALT_BARO_SET_KNB", "-800", "+1600", 4, 3);
DcsBios::RotaryEncoder ehsiHdgSetKnb("EHSI_HDG_SET_KNB", "-800", "+1600", 7,8);
DcsBios::RotaryEncoder ehsiCrsSetKnb("EHSI_CRS_SET_KNB", "-800", "+1600", 9,10);
DcsBios::RotaryEncoder rwrIndDimKnb("RWR_IND_DIM_KNB", "-800", "+1600", 11,12);

//  Buttons
DcsBios::Switch2Pos ehsiMode("EHSI_MODE", A0);
DcsBios::Switch2Pos ehsiCrsSet("EHSI_CRS_SET", A1);
DcsBios::Switch2Pos ehsiHdgSetBtn("EHSI_HDG_SET_BTN", A2);

// The altimeter mode lever is handled by pressing the baro set knob (long)
// long clicks points it in alternating directions, and release sets it middle
void handle_altmode_press(Button2& button) {
  // We've set 0, emit 2 and set current
  if(altmode_current == 0) {
    DcsBios::sendDcsBiosMessage("ALT_MODE_LV", "2");
    altmode_current = 2;
  // Else if were at 2, 
  } else if (altmode_current == 2) {
    DcsBios::sendDcsBiosMessage("ALT_MODE_LV", "0");
    altmode_current = 0;
  }
}

void handle_altmode_release(Button2& button) {
  DcsBios::sendDcsBiosMessage("ALT_MODE_LV", "1");
}

void setup() {
  altmodelever.begin(ALTMODEPIN);
  altmodelever.setPressedHandler(handle_altmode_press);
  altmodelever.setReleasedHandler(handle_altmode_release);
  DcsBios::setup();
}

void loop() {
  altmodelever.loop();
  DcsBios::loop();
}
