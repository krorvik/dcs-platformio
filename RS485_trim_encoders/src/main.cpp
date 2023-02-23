#define DCSBIOS_RS485_SLAVE 118
#define TXENABLE_PIN 2
#include "DcsBios.h"

DcsBios::RotaryEncoder pitchTrim("PITCH_TRIM", "-3200", "+3200", 3, 4);
DcsBios::RotaryEncoder rollTrim("ROLL_TRIM", "-3200", "+3200", 5, 6);

void setup() {
    DcsBios::setup();
}

void loop() {
    DcsBios::loop();
}