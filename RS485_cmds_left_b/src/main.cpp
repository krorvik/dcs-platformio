#define DCSBIOS_RS485_SLAVE 66
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"
#include <TM1637TinyDisplay6.h> 


#define CLK A4 //pins definitions for TM1637 and can be changed to other ports
#define DIO A5
#define MODE_PIN A6
#define PRGM_PIN A7

unsigned int last_mode = 128;
unsigned int last_prgm = 128;

char* display_chars = "      ";

TM1637TinyDisplay6  display(CLK, DIO);

void onCmdsChAmountChange(char* newValue) {
  display_chars[2] = newValue[3];
  display_chars[1] = newValue[2];
}

void onCmdsFlAmountChange(char* newValue) {
  display_chars[5] = newValue[3];
  display_chars[4] = newValue[2];
}

void update_display() {
  display.showString(display_chars);
  // debug to find analog multiswitch values: 
  // display.showNumberDec(last_mode + 1000 * last_prgm);
}

void loop_mode() {
  unsigned int new_mode = analogRead(MODE_PIN) / 16;
  if (new_mode != last_mode) {    
    switch (new_mode) {
      case 0: 
        sendDcsBiosMessage("CMDS_MODE_KNB", "0"); 
        last_mode = new_mode;
        break;
      case 5:
        sendDcsBiosMessage("CMDS_MODE_KNB", "1");
        last_mode = new_mode;
        break;
      case 11:
        sendDcsBiosMessage("CMDS_MODE_KNB", "2");
        last_mode = new_mode;
        break;
      case 17:
        sendDcsBiosMessage("CMDS_MODE_KNB", "3");
        last_mode = new_mode;
        break;
      case 23:
        sendDcsBiosMessage("CMDS_MODE_KNB", "4");
        last_mode = new_mode;
        break;
      case 29:
        sendDcsBiosMessage("CMDS_MODE_KNB", "5");
        last_mode = new_mode;
        break; 
    }
  }
};

void loop_prgm() {
  unsigned int new_prgm = analogRead(PRGM_PIN) / 16;
  if (new_prgm != last_prgm) {    
    switch (new_prgm) {
      case 34: 
        sendDcsBiosMessage("CMDS_PROG_KNB", "0"); 
        last_prgm = new_prgm;
        break;
      case 40:
        sendDcsBiosMessage("CMDS_PROG_KNB", "1");
        last_prgm = new_prgm;
        break;
      case 46:
        sendDcsBiosMessage("CMDS_PROG_KNB", "2");
        last_prgm = new_prgm;
        break;
      case 58:
        sendDcsBiosMessage("CMDS_PROG_KNB", "3");
        last_prgm = new_prgm;
        break;
      case 63:
        sendDcsBiosMessage("CMDS_PROG_KNB", "4");
        last_prgm = new_prgm;
        break;
    }
  }
};

DcsBios::StringBuffer<4> cmdsChAmountBuffer(CMDS_CH_Amount_DISPLAY_ADDRESS, onCmdsChAmountChange);
DcsBios::StringBuffer<4> cmdsFlAmountBuffer(CMDS_FL_Amount_DISPLAY_ADDRESS, onCmdsFlAmountChange);

// LED strip
DcsBios::LED lightCmdsNoGo1(LIGHT_CMDS_NO_GO_LED_ADDRESS, LIGHT_CMDS_NO_GO_LED_ADDRESS, 3);
DcsBios::LED lightCmdsNoGo0(LIGHT_CMDS_NO_GO_LED_ADDRESS, LIGHT_CMDS_NO_GO_LED_ADDRESS, 4);
DcsBios::LED lightCmdsGo(LIGHT_CMDS_GO_LED_ADDRESS, LIGHT_CMDS_GO_LED_MASK, 5);
DcsBios::LED lightCmdsDisp0(LIGHT_CMDS_DISP_LED_ADDRESS, LIGHT_CMDS_DISP_LED_MASK, 6);
DcsBios::LED lightCmdsDisp1(LIGHT_CMDS_DISP_LED_ADDRESS, LIGHT_CMDS_DISP_LED_MASK, 7);
DcsBios::LED lightCmdsRdy(LIGHT_CMDS_RDY_LED_ADDRESS, LIGHT_CMDS_RDY_LED_MASK, 8);

// Switches
// DcsBios::Switch2Pos cmds01ExpCatSw("CMDS_01_EXP_CAT_SW", F6); NOT USED IN BL50
// DcsBios::Switch2Pos cmds02ExpCatSw("CMDS_02_EXP_CAT_SW", F5); NOT USED IN BL50
DcsBios::Switch2Pos cmdsChExpCatSw("CMDS_CH_EXP_CAT_SW", A1);
DcsBios::Switch2Pos cmdsFlExpCatSw("CMDS_FL_EXP_CAT_SW", A3); // = A0
DcsBios::Switch2Pos cmdsJettSw("CMDS_JETT_SW", A0); // = A1
// DcsBios::Switch2Pos cmdsMwsSourcheSw("CMDS_MWS_SOURCE_SW", F12); NOT USED IN BL50
DcsBios::Switch2Pos cmdsJmrSourcheSw("CMDS_JMR_SOURCE_SW", A2); // = A2
DcsBios::Switch2Pos cmdsPwrSourcheSw("CMDS_RWR_SOURCE_SW", 12); // = A3

// Light all lights in series
void debug() {
  for (int i = 3; i<= 8; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
    delay(1000);
    digitalWrite(i, LOW);
  }
}

// Init helper
void initialize() {
  display.setBrightness(BRIGHT_HIGH);
  display.showString(" -- --");
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
  loop_prgm();
  loop_mode();
}


