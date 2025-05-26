#define DCSBIOS_RS485_SLAVE 119
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "Button2.h"

const byte fuelQtySelKnbPins[5] = {7,6,5,4,3};
DcsBios::SwitchMultiPos fuelQtySelKnb("FUEL_QTY_SEL_KNB", fuelQtySelKnbPins, 5);

// The test function in DCS is not mapped like a normal switch. m
// Handle manually.
Button2 test_switch;

void test_pressed(Button2 &button) {
    sendDcsBiosMessage("FUEL_QTY_SEL_T_KNB", "0");
}

void test_released(Button2 &button) {
    sendDcsBiosMessage("FUEL_QTY_SEL_KNB", "0");
}

DcsBios::Switch2Pos extFuelTransSw("EXT_FUEL_TRANS_SW", 9);

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