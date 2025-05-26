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




void setup() {
  initalize();
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}
