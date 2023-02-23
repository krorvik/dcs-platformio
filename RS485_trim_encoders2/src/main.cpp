#define DCSBIOS_RS485_SLAVE 117
#define TXENABLE_PIN 2
#include "DcsBios.h"

DcsBios::RotaryEncoderT<POLL_EVERY_TIME, DcsBios::FOUR_STEPS_PER_DETENT> pitchTrim("PITCH_TRIM", "-3200", "+3200", 4, 3);
DcsBios::RotaryEncoderT<POLL_EVERY_TIME, DcsBios::FOUR_STEPS_PER_DETENT>  rollTrim("ROLL_TRIM", "-3200", "+3200", 5, 6);

void setup() {
    DcsBios::setup();
}

void loop() {
    DcsBios::loop();
}