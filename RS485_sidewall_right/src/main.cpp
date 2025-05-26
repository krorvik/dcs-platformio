#define DCSBIOS_RS485_SLAVE 46
#define TXENABLE_PIN 2
#include "DcsBios.h"

// 3+4 seat adjust 
DcsBios::Switch3Pos seatAdj("SEAT_ADJ", 3 , 4);
DcsBios::Switch2Pos seatEjectSafe("SEAT_EJECT_SAFE", 5);

void setup() {
    DcsBios::setup();
}

void loop() {
    DcsBios::loop();
}