#define DCSBIOS_RS485_SLAVE 43
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include <f16c_data.h>
#include <U8g2lib.h>

#define PNEU_PIN 9

U8G2_SSD1306_64X32_1F_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);


char baro_digits[5] = {'2', '9', '9', '2', '\0'};

void updatedisplay() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_inr16_mf);
  u8g2.drawStr(4,24, baro_digits);
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

void onAltPneuFlagChange(unsigned int newValue) {
    if (newValue > 32768) { 
      digitalWrite(PNEU_PIN, 0); 
    } else {
      digitalWrite(PNEU_PIN, 1); 
    }
}

void onUpdateCounterChange(unsigned int newValue) {
  updatedisplay();
}

void onAltPressureDrum0CntChange(unsigned int newValue) { baro_digits[3] = '0' + translate_digit(newValue); }
void onAltPressureDrum1CntChange(unsigned int newValue) { baro_digits[2] = '0' + translate_digit(newValue); }
void onAltPressureDrum2CntChange(unsigned int newValue) { baro_digits[1] = '0' + translate_digit(newValue); }
void onAltPressureDrum3CntChange(unsigned int newValue) { baro_digits[0] = '0' + translate_digit(newValue); }

DcsBios::IntegerBuffer altPressureDrum0CntBuffer(ALT_PRESSURE_DRUM_0_CNT_GAUGE_ADDRESS , ALT_PRESSURE_DRUM_0_CNT_GAUGE_MASK , ALT_PRESSURE_DRUM_0_CNT_GAUGE_SHIFTBY , onAltPressureDrum0CntChange);
DcsBios::IntegerBuffer altPressureDrum1CntBuffer(ALT_PRESSURE_DRUM_1_CNT_GAUGE_ADDRESS , ALT_PRESSURE_DRUM_1_CNT_GAUGE_MASK , ALT_PRESSURE_DRUM_1_CNT_GAUGE_SHIFTBY , onAltPressureDrum1CntChange);
DcsBios::IntegerBuffer altPressureDrum2CntBuffer(ALT_PRESSURE_DRUM_2_CNT_GAUGE_ADDRESS , ALT_PRESSURE_DRUM_2_CNT_GAUGE_MASK , ALT_PRESSURE_DRUM_2_CNT_GAUGE_SHIFTBY , onAltPressureDrum2CntChange);
DcsBios::IntegerBuffer altPressureDrum3CntBuffer(ALT_PRESSURE_DRUM_3_CNT_GAUGE_ADDRESS , ALT_PRESSURE_DRUM_3_CNT_GAUGE_MASK , ALT_PRESSURE_DRUM_3_CNT_GAUGE_SHIFTBY , onAltPressureDrum3CntChange);
DcsBios::IntegerBuffer altPneuFlagBuffer(ALT_PNEU_FLAG_GAUGE_ADDRESS, ALT_PNEU_FLAG_GAUGE_MASK, ALT_PNEU_FLAG_GAUGE_SHIFTBY , onAltPneuFlagChange);

DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);

void setup() {
  pinMode(PNEU_PIN, OUTPUT);
  digitalWrite(PNEU_PIN, 1);
  DcsBios::setup();
  u8g2.begin();
  updatedisplay();
}

void loop() {
  DcsBios::loop();
  updatedisplay();
}
