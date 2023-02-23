#define DCSBIOS_RS485_SLAVE 119
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include <Button2.h>

#define BARO_BUTTON_PIN A3

Button2 baroButton;
unsigned int current_barobutton_dir = 1;

// 3-9 EHSI
DcsBios::RotaryEncoder ehsiHdgSetKnb("EHSI_HDG_SET_KNB", "-3200", "+3200", 5,6);
DcsBios::RotaryEncoder ehsiCrsSetKnb("EHSI_CRS_SET_KNB", "-3200", "+3200", 3,4);

DcsBios::Switch2Pos ehsiMode("EHSI_MODE", 7);
DcsBios::Switch2Pos ehsiHdgSetBtn("EHSI_HDG_SET_BTN", 9);
DcsBios::Switch2Pos ehsiCrsSet("EHSI_CRS_SET", 8);
// 10 - A2 fuel qty panel
// not the reverse on the test function, since this switch is wired to go up on selection. 
DcsBios::Switch2Pos fuelQtySelTKnb("FUEL_QTY_SEL_T_KNB", A1, true);

const byte fuelQtySelKnbPins[5] = {A0, 13, 12, 11, 10};
DcsBios::SwitchMultiPos fuelQtySelKnb("FUEL_QTY_SEL_KNB", fuelQtySelKnbPins, 5);

DcsBios::Switch2Pos extFuelTransSw("EXT_FUEL_TRANS_SW", A2);

// Baro pressure knob on A3, A4, button on A5
DcsBios::RotaryEncoder altBaroSetKnb("ALT_BARO_SET_KNB", "-3200", "+3200", A4, A5);

void baroButtonReleaseHandler(Button2 &button) {
  // Invert direction
  current_barobutton_dir = current_barobutton_dir * -1;
  // Reset to 1 / middle pos
  DcsBios::tryToSendDcsBiosMessage("ALT_MODE_LV", "1");
}

void baroButtonPressHandler(Button2 &button) {
  char dir[] = { current_barobutton_dir + '1' };
  // Set to current diretion, which is center plus dir (1 - 1 or 1 + 1))
  DcsBios::tryToSendDcsBiosMessage("ALT_MODE_LV", dir);
}

void setup() {
    baroButton.begin(BARO_BUTTON_PIN);
    baroButton.setPressedHandler(baroButtonPressHandler);
    baroButton.setReleasedHandler(baroButtonReleaseHandler);
    DcsBios::setup();
}

void loop() {
    baroButton.loop();
    DcsBios::loop();
}