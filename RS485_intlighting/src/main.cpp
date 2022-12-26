#define DCSBIOS_RS485_SLAVE 120
#define TXENABLE_PIN 2
#include <DcsBios.h>
#include <f16c_data.h>
#include <common_data.h>

#define PRI_CONSOLE_OUT 5
#define PRI_INSTR_OUT 6
#define FLOOD_CONSOLE_OUT 10
#define FLOOD_INSTR_OUT 11 

void onPriConsolesBrtKnbChange(unsigned int newValue) { analogWrite(PRI_CONSOLE_OUT, map(newValue, 0, 65535, 0, 255)); }
void onPriInstPnlBrtKnbChange(unsigned int newValue) { analogWrite(PRI_INSTR_OUT, map(newValue, 0, 65535, 0, 255)); }
void onFloodConsolesBrtKnbChange(unsigned int newValue) { analogWrite(FLOOD_CONSOLE_OUT, map(newValue, 0, 65535, 0, 255)); }
void onFloodInstPnlBrtKnbChange(unsigned int newValue) { analogWrite(FLOOD_INSTR_OUT, map(newValue, 0, 65535, 0, 255)); }

DcsBios::IntegerBuffer priConsolesBrtKnbBuffer(PRI_CONSOLES_BRT_KNB_LIMITDIAL_ADDRESS, PRI_CONSOLES_BRT_KNB_LIMITDIAL_MASK, PRI_CONSOLES_BRT_KNB_LIMITDIAL_SHIFTBY, onPriConsolesBrtKnbChange);
DcsBios::IntegerBuffer priInstPnlBrtKnbBuffer(PRI_INST_PNL_BRT_KNB_LIMITDIAL_ADDRESS, PRI_INST_PNL_BRT_KNB_LIMITDIAL_MASK, PRI_INST_PNL_BRT_KNB_LIMITDIAL_SHIFTBY, onPriInstPnlBrtKnbChange);
DcsBios::IntegerBuffer floodConsolesBrtKnbBuffer(FLOOD_CONSOLES_BRT_KNB_LIMITDIAL_ADDRESS, FLOOD_CONSOLES_BRT_KNB_LIMITDIAL_MASK, FLOOD_CONSOLES_BRT_KNB_LIMITDIAL_SHIFTBY, onFloodConsolesBrtKnbChange);
DcsBios::IntegerBuffer floodInstPnlBrtKnbBuffer(FLOOD_INST_PNL_BRT_KNB_LIMITDIAL_ADDRESS, FLOOD_INST_PNL_BRT_KNB_LIMITDIAL_MASK, FLOOD_INST_PNL_BRT_KNB_LIMITDIAL_SHIFTBY, onFloodInstPnlBrtKnbChange);

void reset(unsigned int level) {
  analogWrite(PRI_CONSOLE_OUT, level);
  analogWrite(PRI_INSTR_OUT, level);
  analogWrite(FLOOD_CONSOLE_OUT, level);
  analogWrite(FLOOD_INSTR_OUT, level);
}

void setup() {
  delay(1000);
  pinMode(PRI_CONSOLE_OUT, OUTPUT);
  pinMode(PRI_INSTR_OUT, OUTPUT);
  pinMode(FLOOD_CONSOLE_OUT, OUTPUT);
  pinMode(FLOOD_INSTR_OUT, OUTPUT);  
  reset(64);
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}