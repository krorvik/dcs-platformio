#define DCSBIOS_RS485_MASTER
#define DCSBIOS_DISABLE_SERVO
#define UART1_TXENABLE_PIN 2
#define UART2_TXENABLE_PIN 3
#define UART3_TXENABLE_PIN 4

#include "DcsBios.h"

void setup() {
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
