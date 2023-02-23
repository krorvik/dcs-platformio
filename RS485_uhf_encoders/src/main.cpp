#define DCSBIOS_RS485_SLAVE 115
#define TXENABLE_PIN 2
#include "DcsBios.h"

// DcsBios::RotaryEncoderT<POLL_EVERY_TIME, DcsBios::FOUR_STEPS_PER_DETENT> pitchTrim("PITCH_TRIM", "-3200", "+3200", 4, 3);

DcsBios::RotaryEncoder uhfChanKnb("UHF_CHAN_KNB", "DEC", "INC", A1, A0);

DcsBios::RotaryEncoder uhfFreq0025Knb("UHF_FREQ_0025_KNB", "DEC", "INC", 12, 11);
DcsBios::RotaryEncoder uhfFreq01Knb("UHF_FREQ_01_KNB", "DEC", "INC", 4, 3);
DcsBios::RotaryEncoder uhfFreq1Knb("UHF_FREQ_1_KNB", "DEC", "INC", 6, 5);
DcsBios::RotaryEncoder uhfFreq10Knb("UHF_FREQ_10_KNB", "DEC", "INC", 8,7);
DcsBios::RotaryEncoder uhfFreq100Knb("UHF_FREQ_100_KNB", "DEC", "INC", 10, 9);


void setup() {
    DcsBios::setup();
}

void loop() {
    DcsBios::loop();
}