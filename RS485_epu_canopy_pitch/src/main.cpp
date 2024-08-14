#define DCSBIOS_RS485_SLAVE 81
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"

void initalize() {
  // Flash leds
  for (int i = 5; i<= 7; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
    delay(1000);
    digitalWrite(i, LOW);
  }
}

// Switches
DcsBios::Switch3Pos epuSw("EPU_SW", 3, 4);
//DcsBios::Switch3Pos canopySw("CANOPY_SW", 5, 7);
//DcsBios::Switch2Pos canopyHandle("CANOPY_HANDLE", 8, true);  // last param: invert switch 
//DcsBios::Switch2Pos seatEjectSafe("SEAT_EJECT_SAFE", 6, true);  // last param: invert switch
//DcsBios::Switch2Pos manualPitchSw("MANUAL_PITCH_SW", 9);

// LEDs
DcsBios::LED lightHydrazn(LIGHT_HYDRAZN_LED_ADDRESS, LIGHT_HYDRAZN_LED_MASK, 5);
DcsBios::LED lightAir(LIGHT_AIR_LED_ADDRESS, LIGHT_AIR_LED_MASK, 6);
DcsBios::LED lightEpu(LIGHT_EPU_LED_ADDRESS, LIGHT_EPU_LED_MASK, 7);

void setup() {
  initalize();
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}
