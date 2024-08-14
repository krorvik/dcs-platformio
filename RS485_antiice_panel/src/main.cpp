#define DCSBIOS_RS485_SLAVE 93
#define TXENABLE_PIN 2
#include <DcsBios.h>

DcsBios::Switch3Pos iffAntSelSw("IFF_ANT_SEL_SW", 4, 3);
DcsBios::Switch3Pos uhfAntSelSw("UHF_ANT_SEL_SW", 7, 6);
DcsBios::Switch3Pos engAntiIce("ENG_ANTI_ICE", 5, 8);

void setup() {
  // put your setup code here, to run once:
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}