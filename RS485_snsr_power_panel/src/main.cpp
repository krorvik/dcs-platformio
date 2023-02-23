#define DCSBIOS_RS485_SLAVE 94
#define TXENABLE_PIN 2
#include <DcsBios.h>

DcsBios::Switch2Pos hdptSwL("HDPT_SW_L", 7);
DcsBios::Switch2Pos hdptSwR("HDPT_SW_R", 6);
DcsBios::Switch2Pos fcrPwrSw("FCR_PWR_SW", 5);
DcsBios::Switch3Pos rdrAltPwrSw("RDR_ALT_PWR_SW", 4, 3);

void setup() {
  // put your setup code here, to run once:
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}