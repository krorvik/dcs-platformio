#define DCSBIOS_RS485_SLAVE 116
#define TXENABLE_PIN 2
#include <DcsBios.h>
#include <f16c_data.h>

#define PRI_CONSOLE_OUT 5
#define PRI_INSTR_OUT 6
#define FLOOD_CONSOLE_OUT 10
#define FLOOD_INSTR_OUT 11 

void onPriConsolesBrtKnbChange(unsigned int newValue)   { analogWrite(PRI_CONSOLE_OUT,   map(newValue, 0, 65535, 0, 255));   }
void onPriInstPnlBrtKnbChange(unsigned int newValue)    { analogWrite(PRI_INSTR_OUT,     map(newValue, 0, 65535, 0, 255));   } 
void onFloodConsolesBrtKnbChange(unsigned int newValue) { analogWrite(FLOOD_CONSOLE_OUT, map(newValue, 0, 65535, 0, 255));   }
void onFloodInstPnlBrtKnbChange(unsigned int newValue)  { analogWrite(FLOOD_INSTR_OUT,   map(newValue, 0, 65535, 0, 255));   }

DcsBios::IntegerBuffer priConsolesBrtKnbBuffer  (LIGHT_CONSLES_GAUGE_ADDRESS,        LIGHT_CONSLES_GAUGE_MASK,        LIGHT_CONSLES_GAUGE_SHIFTBY,        onPriConsolesBrtKnbChange);
DcsBios::IntegerBuffer priInstPnlBrtKnbBuffer   (LIGHT_INST_PNL_GAUGE_ADDRESS,       LIGHT_INST_PNL_GAUGE_MASK,       LIGHT_INST_PNL_GAUGE_SHIFTBY,       onPriInstPnlBrtKnbChange);
DcsBios::IntegerBuffer floodConsolesBrtKnbBuffer(LIGHT_CONSLES_FLOOD_GAUGE_ADDRESS,  LIGHT_CONSLES_FLOOD_GAUGE_MASK,  LIGHT_CONSLES_FLOOD_GAUGE_SHIFTBY,  onFloodConsolesBrtKnbChange);
DcsBios::IntegerBuffer floodInstPnlBrtKnbBuffer (LIGHT_INST_PNL_FLOOD_GAUGE_ADDRESS, LIGHT_INST_PNL_FLOOD_GAUGE_MASK, LIGHT_INST_PNL_FLOOD_GAUGE_SHIFTBY, onFloodInstPnlBrtKnbChange);

void reset(unsigned int level) {
  analogWrite(PRI_CONSOLE_OUT, level);
  analogWrite(PRI_INSTR_OUT, level);
  analogWrite(FLOOD_CONSOLE_OUT, level);
  analogWrite(FLOOD_INSTR_OUT, level);
}

void blink()  {
  for (unsigned i = 0; i < 5; i++) {
    reset(255);
    delay(1000);
    reset(0);
  }
}

void setup() {
  pinMode(PRI_CONSOLE_OUT, OUTPUT);
  pinMode(PRI_INSTR_OUT, OUTPUT);
  pinMode(FLOOD_CONSOLE_OUT, OUTPUT);
  pinMode(FLOOD_INSTR_OUT, OUTPUT);  
  blink();
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}