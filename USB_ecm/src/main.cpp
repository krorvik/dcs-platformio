// For RS485
#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
#include "f16c_data.h"  // This is a generated header with all addresses. Rewriting this file is not fun. 

// Button 1 LEDs
//FSAT
DcsBios::LED lightEcm1F(LIGHT_ECM_1_F_LED_ADDRESS, LIGHT_ECM_1_F_LED_MASK, 4);
DcsBios::LED lightEcm1S(LIGHT_ECM_1_S_LED_ADDRESS, LIGHT_ECM_1_S_LED_MASK, 5);
DcsBios::LED lightEcm1A(LIGHT_ECM_1_A_LED_ADDRESS, LIGHT_ECM_1_A_LED_MASK, 6);
DcsBios::LED lightEcm1T(LIGHT_ECM_1_T_LED_ADDRESS, LIGHT_ECM_1_T_LED_MASK, 7);

// Button 2 LEDs
DcsBios::LED lightEcm2F(LIGHT_ECM_2_F_LED_ADDRESS, LIGHT_ECM_2_F_LED_MASK, 8);
DcsBios::LED lightEcm2S(LIGHT_ECM_2_S_LED_ADDRESS, LIGHT_ECM_2_S_LED_MASK, 9);
DcsBios::LED lightEcm2A(LIGHT_ECM_2_A_LED_ADDRESS, LIGHT_ECM_2_A_LED_MASK, 10);
DcsBios::LED lightEcm2T(LIGHT_ECM_2_T_LED_ADDRESS, LIGHT_ECM_2_T_LED_MASK, 11);

// Button 3 LEDs
DcsBios::LED lightEcm3F(LIGHT_ECM_3_F_LED_ADDRESS, LIGHT_ECM_3_F_LED_MASK, 12);
DcsBios::LED lightEcm3S(LIGHT_ECM_3_S_LED_ADDRESS, LIGHT_ECM_3_S_LED_MASK, 13);
DcsBios::LED lightEcm3A(LIGHT_ECM_3_A_LED_ADDRESS, LIGHT_ECM_3_A_LED_MASK, 14);
DcsBios::LED lightEcm3T(LIGHT_ECM_3_T_LED_ADDRESS, LIGHT_ECM_3_T_LED_MASK, 15);

// Button 4 543
DcsBios::LED lightEcm4F(LIGHT_ECM_4_F_LED_ADDRESS, LIGHT_ECM_4_F_LED_MASK, 16);
DcsBios::LED lightEcm4S(LIGHT_ECM_4_S_LED_ADDRESS, LIGHT_ECM_4_S_LED_MASK, 17);
DcsBios::LED lightEcm4A(LIGHT_ECM_4_A_LED_ADDRESS, LIGHT_ECM_4_A_LED_MASK, 18);
DcsBios::LED lightEcm4T(LIGHT_ECM_4_T_LED_ADDRESS, LIGHT_ECM_4_T_LED_MASK, 19);

// Button 5 LEDs
// SFTA
DcsBios::LED lightEcm5F(LIGHT_ECM_5_F_LED_ADDRESS, LIGHT_ECM_5_F_LED_MASK, 21);
DcsBios::LED lightEcm5S(LIGHT_ECM_5_S_LED_ADDRESS, LIGHT_ECM_5_S_LED_MASK, 20);
DcsBios::LED lightEcm5A(LIGHT_ECM_5_A_LED_ADDRESS, LIGHT_ECM_5_A_LED_MASK, 23);
DcsBios::LED lightEcm5T(LIGHT_ECM_5_T_LED_ADDRESS, LIGHT_ECM_5_F_LED_MASK, 22);

// Button ALT LEDs
DcsBios::LED lightEcmF(LIGHT_ECM_F_LED_ADDRESS, LIGHT_ECM_F_LED_MASK, 25);
DcsBios::LED lightEcmS(LIGHT_ECM_S_LED_ADDRESS, LIGHT_ECM_S_LED_MASK, 24);
DcsBios::LED lightEcmA(LIGHT_ECM_A_LED_ADDRESS, LIGHT_ECM_A_LED_MASK, 27);
DcsBios::LED lightEcmT(LIGHT_ECM_T_LED_ADDRESS, LIGHT_ECM_T_LED_MASK, 26);

// Button FRM LEDs
DcsBios::LED lightEcmFrmF(LIGHT_ECM_FRM_F_LED_ADDRESS, LIGHT_ECM_FRM_F_LED_MASK, 29);
DcsBios::LED lightEcmFrmS(LIGHT_ECM_FRM_S_LED_ADDRESS, LIGHT_ECM_FRM_S_LED_MASK, 28);
DcsBios::LED lightEcmFrmA(LIGHT_ECM_FRM_A_LED_ADDRESS, LIGHT_ECM_FRM_A_LED_MASK, 31);
DcsBios::LED lightEcmFrmT(LIGHT_ECM_FRM_T_LED_ADDRESS, LIGHT_ECM_FRM_T_LED_MASK, 30);

// Button SPL LEDs
DcsBios::LED lightEcmSplF(LIGHT_ECM_SPL_F_LED_ADDRESS, LIGHT_ECM_SPL_F_LED_MASK, 33);
DcsBios::LED lightEcmSplS(LIGHT_ECM_SPL_S_LED_ADDRESS, LIGHT_ECM_SPL_S_LED_MASK, 32);
DcsBios::LED lightEcmSplA(LIGHT_ECM_SPL_A_LED_ADDRESS, LIGHT_ECM_SPL_A_LED_MASK, 35);
DcsBios::LED lightEcmSplT(LIGHT_ECM_SPL_T_LED_ADDRESS, LIGHT_ECM_SPL_T_LED_MASK, 34);


//Switches
//Modules
DcsBios::Switch2Pos ecm1Btn("ECM_1_BTN", 36);
DcsBios::Switch2Pos ecm2Btn("ECM_2_BTN", 37);
DcsBios::Switch2Pos ecm3Btn("ECM_3_BTN", 38);
DcsBios::Switch2Pos ecm4Btn("ECM_4_BTN", 39);
DcsBios::Switch2Pos ecm5Btn("ECM_5_BTN", 40);
DcsBios::Switch2Pos ecm6Btn("ECM_6_BTN", 41);
DcsBios::Switch2Pos ecmFRMBtn("ECM_FRM_BTN", 42);
DcsBios::Switch2Pos ecmSPLBtn("ECM_SPL_BTN", 43);

// Toggles
DcsBios::Switch3Pos ecmPwSw("ECM_PW_SW", 50, 51);
DcsBios::Switch3Pos ecmXmitSw("ECM_XMIT_SW", 53, 49);

//Push
DcsBios::Switch2Pos ecmBitBtn("ECM_BIT_BTN", 48); // OK
DcsBios::Switch2Pos ecmResetBtn("ECM_RESET_BTN", 52);

// Dim
// DcsBios::Potentiometer ecmDimKnb("ECM_DIM_KNB", A0); // OK

void initialize() {
  for (int i = 4; i<36; i++) {
    digitalWrite(i, HIGH);
  }
  delay(2000);
  for (int i = 4; i<36; i++) {
    digitalWrite(i, LOW);
  }
}

void setup() {
  initialize();
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
