#define DCSBIOS_RS485_SLAVE 80
#define TXENABLE_PIN 2
#include "DcsBios.h"

void initalize() {
  for (int i = 3; i <= 4; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  for (int i = A0; i <= A2; i++) {
    pinMode(10, INPUT);
  }
}

DcsBios::Potentiometer intercomKnb("INTERCOM_KNB", A0);
DcsBios::Potentiometer tacanKnb("TACAN_KNB", A1);
DcsBios::Potentiometer ilsPwrKnb("ILS_PWR_KNB", A2);
DcsBios::Switch3Pos hotMicSw("HOT_MIC_SW", 3,4);

void setup() {
  initalize();
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}
