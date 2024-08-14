#define DCSBIOS_RS485_SLAVE 119
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "Button2.h"

const byte fuelQtySelKnbPins[5] = {7,6,5,4,3};
DcsBios::SwitchMultiPos fuelQtySelKnb("FUEL_QTY_SEL_KNB", fuelQtySelKnbPins, 5);

// Can't handle like this. Must have custom switch that sets fuel sel back to 0 when off
// DcsBios::Switch2Pos fuelQtySelTKnb("FUEL_QTY_SEL_T_KNB", 8);
Button2 test_switch;

void test_pressed(Button2 &button) {
    sendDcsBiosMessage("FUEL_QTY_SEL_T_KNB", "0");
}

void test_released(Button2 &button) {
    sendDcsBiosMessage("FUEL_QTY_SEL_KNB", "0");
}


DcsBios::Switch2Pos extFuelTransSw("EXT_FUEL_TRANS_SW", 9);

// 3-9 EHSI
DcsBios::RotaryEncoder ehsiHdgSetKnb("EHSI_HDG_SET_KNB", "-3200", "+3200", 16,15);
DcsBios::RotaryEncoder ehsiCrsSetKnb("EHSI_CRS_SET_KNB", "-3200", "+3200", 13,14);

DcsBios::Switch2Pos ehsiMode("EHSI_MODE", 12);
DcsBios::Switch2Pos ehsiHdgSetBtn("EHSI_HDG_SET_BTN", 10);
DcsBios::Switch2Pos ehsiCrsSet("EHSI_CRS_SET", 11);

void setup() {
    test_switch.begin(8); // button_pin
    test_switch.setPressedHandler(test_pressed);
    test_switch.setReleasedHandler(test_released);
    DcsBios::setup();
}

void loop() {
    test_switch.loop();
    DcsBios::loop();
}