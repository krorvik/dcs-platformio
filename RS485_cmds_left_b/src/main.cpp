#define DCSBIOS_RS485_SLAVE 66
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"
#include <TM1637TinyDisplay6.h> 

#define CLK 11 //pins definitions for TM1637 and can be changed to other ports
#define DIO 12

TM1637TinyDisplay6  tm1637_6D(CLK, DIO);

void onUhfFreqDispChange(char* newValue) {
    // Known non-decimal values
    char stars[] = "***.***";
    if (strcmp(newValue, stars) == 0) { 
      tm1637_6D.showString("------");
    } else if (strcmp(newValue, "       ") == 0) { 
      tm1637_6D.clear(); 
    } else {
      // Float, display it
      float value = atof(newValue);
      tm1637_6D.showNumber(value);
    }
}

DcsBios::StringBuffer<7> uhfFreqDispBuffer(UHF_FREQ_DISP_DISPLAY_ADDRESS, onUhfFreqDispChange);
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

  tm1637_6D.setBrightness(BRIGHT_HIGH);
  tm1637_6D.showNumber(888888);
 
  delay(2000);
 
  tm1637_6D.clear(); 
}


void setup() {
  initialize();  
  DcsBios::setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
  
}


