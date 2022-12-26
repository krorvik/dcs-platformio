#define DCSBIOS_RS485_SLAVE 68
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"

unsigned int eng_cont_last_pos = 2;  // start pointing up, 0 = down, 1 = middle, 2 = up

unsigned int getContPos() {
  unsigned int new_upper = digitalRead(6);
  unsigned int new_lower = digitalRead(7);
  if (new_upper == 0) {
    return 2;
  } else if (new_lower == 0) {
    return 0;
  } else if (new_upper == 1 and new_lower == 1) { 
    return 1;
  }
}

void evalCont() {
  unsigned int new_pos = getContPos();
  // Only do something if it moved
  if (new_pos != eng_cont_last_pos) {
    // Moving from covered, PRIMARY - new position should be either uncovered, primary, or uncovered, secondary    
    if(eng_cont_last_pos == 2) {
      if (new_pos == 1) {
        // Cover up
        sendDcsBiosMessage("ENG_CONT_SW_COVER", "1");
      } else if (new_pos == 0) {
        // Cover up
        sendDcsBiosMessage("ENG_CONT_SW_COVER", "1");
        // Cont switch up, PRI
        sendDcsBiosMessage("ENG_CONT_SW", "1");
      }
      // Moving all the way down
      if (new_pos == 0) {
        sendDcsBiosMessage("ENG_CONT_SW_COVER", "1");
        // Cont switch down, SEC
        sendDcsBiosMessage("ENG_CONT_SW", "0");      
      }      
    }
    // Moving from middle - open cover, primary
    else if(eng_cont_last_pos == 1) {
      // Going up - PRI, cover
      if (new_pos == 2) {
        // Cont switch up, PRI
        sendDcsBiosMessage("ENG_CONT_SW", "1");
        // Cover down
        sendDcsBiosMessage("ENG_CONT_SW_COVER", "0");
      // Going down, open cover, into SEC
      } else if (new_pos == 0) {
        // Cont sw to SEC
        sendDcsBiosMessage("ENG_CONT_SW", "0");
        // Cover up
        sendDcsBiosMessage("ENG_CONT_SW_COVER", "1");        
      }      
    }
    // Going from uncovered SEC and up
    else if (eng_cont_last_pos == 0) {
      if (new_pos == 1) {
        // Going to middle, PRI uncovered
        sendDcsBiosMessage("ENG_CONT_SW_COVER", "1");
        // Cont switch up, PRI
        sendDcsBiosMessage("ENG_CONT_SW", "1");
      }
      // All the way up
      else if (new_pos == 2) {
        // Cont switch up, PRI
        sendDcsBiosMessage("ENG_CONT_SW", "1");
        // And close cover
        sendDcsBiosMessage("ENG_CONT_SW_COVER", "0");
      }
    }  
  }
  // And remember position to next iteration:
  eng_cont_last_pos = new_pos;
}

DcsBios::Switch3Pos abResetSw("AB_RESET_SW", 3, 4);
DcsBios::Switch2Pos maxPwrSw("MAX_PWR_SW", 5);
DcsBios::Switch3Pos jfsSw("JFS_SW", 9, 8);
DcsBios::LED lightJfsRun(LIGHT_JFS_RUN_LED_ADDRESS, LIGHT_JFS_RUN_LED_MASK, 10);

void initalize() {
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  delay(1000);
  digitalWrite(10, LOW);  
}

void setup() {
  initalize();
  DcsBios::setup();
}

void loop() {
  evalCont();
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}
