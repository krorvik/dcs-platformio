#define DCSBIOS_RS485_SLAVE 44
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include <f16c_data.h>
#include <U8g2lib.h>

U8G2_SSD1306_64X32_1F_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

char mach_chars[5] = {'0', '.', '0', '0', '\0'};

#define m15_limit 49000
#define m1_limit 60413
#define on_limit 62782

// needed constants
const unsigned int m1_ratio = (m1_limit - m15_limit) * 2;
const unsigned int on_ratio = (on_limit - m1_limit) * 2;

void updatedisplay() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_inr16_mf);
  u8g2.drawStr(4,24, mach_chars);
  u8g2.sendBuffer();
}

int translate_digit(unsigned int value) {
  if (value < 6553) { return 0;}
  if (value < 13107) { return 1;}
  if (value < 19660) { return 2;}
  if (value < 26214) { return 3;}
  if (value < 32767) { return 4;}
  if (value < 39321) { return 5;}
  if (value < 45874) { return 6;}
  if (value < 52428) { return 7;}
  if (value < 58981) { return 8;}
  return 9;
}

float calculate_mach(unsigned int value) {  
  if (value >= on_limit) {
    return 0.5;
  } else if (value >= m1_limit) {
    return 0.5 + (on_limit - (float) value) / on_ratio;
  } else {
    return 1 + (m1_limit - (float) value) / m1_ratio;
  }
}

void onMachIndicatorChange(unsigned int newValue) { dtostrf(calculate_mach(newValue), 4, 2, mach_chars); }
void onUpdateCounterChange(unsigned int newValue) { updatedisplay(); }

DcsBios::IntegerBuffer machIndicatorBuffer      (MACH_INDICATOR_GAUGE_ADDRESS , MACH_INDICATOR_GAUGE_MASK , MACH_INDICATOR_GAUGE_SHIFTBY , onMachIndicatorChange);
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);

void setup() {
  DcsBios::setup();
  u8g2.begin();
  updatedisplay();
}

void loop() {
  DcsBios::loop();
  updatedisplay();
}
