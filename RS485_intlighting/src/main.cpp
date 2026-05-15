#define DCSBIOS_RS485_SLAVE 116
#define TXENABLE_PIN 2
#include <DcsBios.h>
#include <f16c_data.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

unsigned int mainspower_value = 0;

// We will choose the minimum value, which will be this one if zero - power off. Else the actual value below
void onPowerSwitchChange(unsigned int newValue) {
  if (newValue != 0) {
    mainspower_value = 65535;
  } else {
    mainspower_value = 0;
  }
}

// Here, use minimum of mainspower_value and newValue.
void onPriConsolesBrtKnbChange(unsigned int newValue)   { pwm.setPWM(0, 0, map(min(mainspower_value, newValue), 0, 65535, 0, 4096)); }
void onPriInstPnlBrtKnbChange(unsigned int newValue)    { pwm.setPWM(1, 0, map(min(mainspower_value, newValue), 0, 65535, 0, 4096)); } 
void onFloodConsolesBrtKnbChange(unsigned int newValue) { pwm.setPWM(2, 0, map(min(mainspower_value, newValue), 0, 65535, 0, 4096)); }
void onFloodInstPnlBrtKnbChange(unsigned int newValue)  { pwm.setPWM(3, 0, map(min(mainspower_value, newValue), 0, 65535, 0, 4096)); }

DcsBios::IntegerBuffer priConsolesBrtKnbBuffer  (PRI_CONSOLES_BRT_KNB_LIMITDIAL_ADDRESS,    PRI_CONSOLES_BRT_KNB_LIMITDIAL_MASK,    PRI_CONSOLES_BRT_KNB_LIMITDIAL_SHIFTBY,    onPriConsolesBrtKnbChange);
DcsBios::IntegerBuffer priInstPnlBrtKnbBuffer   (PRI_INST_PNL_BRT_KNB_LIMITDIAL_ADDRESS,    PRI_INST_PNL_BRT_KNB_LIMITDIAL_MASK,    PRI_INST_PNL_BRT_KNB_LIMITDIAL_SHIFTBY,    onPriInstPnlBrtKnbChange);
DcsBios::IntegerBuffer floodConsolesBrtKnbBuffer(FLOOD_CONSOLES_BRT_KNB_LIMITDIAL_ADDRESS,  FLOOD_CONSOLES_BRT_KNB_LIMITDIAL_MASK,  FLOOD_CONSOLES_BRT_KNB_LIMITDIAL_SHIFTBY,  onFloodConsolesBrtKnbChange);
DcsBios::IntegerBuffer floodInstPnlBrtKnbBuffer (FLOOD_INST_PNL_BRT_KNB_LIMITDIAL_ADDRESS,  FLOOD_INST_PNL_BRT_KNB_LIMITDIAL_MASK,  FLOOD_INST_PNL_BRT_KNB_LIMITDIAL_SHIFTBY,  onFloodInstPnlBrtKnbChange);
DcsBios::IntegerBuffer mainPwrBuffer            (MAIN_PWR_SW_SELECTOR_ADDRESS,              MAIN_PWR_SW_SELECTOR_MASK,               MAIN_PWR_SW_SELECTOR_SHIFTBY,             onPowerSwitchChange);


void startup_test () {
  for (unsigned int repeats = 0; repeats < 10; repeats++) {
    for(unsigned int channel = 0; channel < 4; channel++) { pwm.setPWM(channel, 0, 4095); }
    delay(1000);
    for(unsigned int channel = 0; channel < 4; channel++) { pwm.setPWM(channel, 0, 0); }
  }
}

void setup() {
  pwm.begin();
  pwm.setPWMFreq(256); 
  startup_test();
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}