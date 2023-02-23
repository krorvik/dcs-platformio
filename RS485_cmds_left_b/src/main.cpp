#define DCSBIOS_RS485_SLAVE 66
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"
#include <TM1637TinyDisplay6.h> 

#define CLK 11 //pins definitions for TM1637 and can be changed to other ports
#define DIO 12

char display_chars[7] = { ' ', '-' , '-', ' ', '-', '-'};

TM1637TinyDisplay6  display(CLK, DIO);

// void onCmdsO1AmountChange(char* newValue) {}    // Not implemented in DCS/block 50
// void onCmdsO2AmountChange(char* newValue) {}    // Not implemented in DCS/block 50
void onCmdsChAmountChange(char* newValue) {
  display_chars[2] = (char*) newValue[3];
  display_chars[1] = (char*) newValue[2];
}

void onCmdsFlAmountChange(char* newValue) {
  display_chars[5] = (char*) newValue[3];
  display_chars[4] = (char*) newValue[2];
}

void update_display() {
  display.showString(display_chars);
}

DcsBios::StringBuffer<4> cmdsChAmountBuffer(CMDS_CH_Amount_DISPLAY_ADDRESS, onCmdsChAmountChange);
DcsBios::StringBuffer<4> cmdsFlAmountBuffer(CMDS_FL_Amount_DISPLAY_ADDRESS, onCmdsFlAmountChange);
// DcsBios::StringBuffer<4> cmdsO1AmountBuffer(CMDS_O1_Amount_DISPLAY_ADDRESS, onCmdsO1AmountChange);  // Not implemented in DCS/block 50
// DcsBios::StringBuffer<4> cmdsO2AmountBuffer(CMDS_O2_Amount_DISPLAY_ADDRESS, onCmdsO2AmountChange);  // Not implemented in DCS/block 50
// Toggle switches

// Rotary switches
const byte cmdsProgKnbPins[5] = {6, 10, 9, 8, 7};
DcsBios::SwitchMultiPos cmdsProgKnb("CMDS_PROG_KNB", cmdsProgKnbPins, 5);

const byte cmdsModeKnbPins[6] = {4, A0, 3, 5, A1, A2};
DcsBios::SwitchMultiPos cmdsModeKnb("CMDS_MODE_KNB", cmdsModeKnbPins, 6);

// Init helper
void initialize() {

  for (int i = 3; i <= 10; i++) {
    pinMode(i, INPUT_PULLUP);
  }

  for (int i = A0; i <= A2; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  display.setBrightness(BRIGHT_HIGH);
  delay(2000);
  display.clear(); 
}

void setup() {
  initialize();  
  DcsBios::setup();
  update_display();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
  update_display();
}


