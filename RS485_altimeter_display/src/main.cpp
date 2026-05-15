#define DCSBIOS_RS485_SLAVE 42
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include <Adafruit_SSD1306.h>
#include <f16c_data.h>
#include <Wire.h>

Adafruit_SSD1306 display(128, 32, &Wire, -1);

char alt_digits[6] = { '0', '0', '-', '0', '0', '\0'};

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

char* alt_checkered() {
  char *tmp_digits = alt_digits;
  if (alt_digits[0] == '0') {
    tmp_digits[0] = 0xB0;
    if (alt_digits[1] == '0' ) { tmp_digits[1] = 0xB0;}
  }
  return tmp_digits;
}

void updatedisplay() {
  display.clearDisplay();
  display.dim(true);
  display.setRotation(2); // 180deg
  display.setTextSize(4);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(5, 2);
  display.println(alt_checkered());
  display.display();
}

void onAlt10000FtCntChange(unsigned int newValue) { 
  alt_digits[0] = '0' + translate_digit(newValue); 
}

void onAlt1000FtCntChange(unsigned int newValue)  { 
  alt_digits[1] = '0' + translate_digit(newValue); 
}

void onAlt100FtCntChange(unsigned int newValue) {
  alt_digits[2] = '0' + translate_digit(newValue);
  // Calculate mods
  alt_digits[3] = '0' + map(newValue % (65535 / 10), 0, 6554, 0, 10);
}

void onUpdateCounterChange(unsigned int newValue) {
  updatedisplay();
}

DcsBios::IntegerBuffer alt10000FtCntBuffer(ALT_10000_FT_CNT_GAUGE_ADDRESS, ALT_10000_FT_CNT_GAUGE_MASK, ALT_10000_FT_CNT_GAUGE_SHIFTBY, onAlt10000FtCntChange);
DcsBios::IntegerBuffer alt1000FtCntBuffer (ALT_1000_FT_CNT_GAUGE_ADDRESS,  ALT_1000_FT_CNT_GAUGE_MASK,  ALT_1000_FT_CNT_GAUGE_SHIFTBY,  onAlt1000FtCntChange );
DcsBios::IntegerBuffer alt100FtCntBuffer  (ALT_100_FT_CNT_GAUGE_ADDRESS,   ALT_100_FT_CNT_GAUGE_MASK,   ALT_100_FT_CNT_GAUGE_SHIFTBY,   onAlt100FtCntChange  );
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);

void setup() {  
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  updatedisplay();
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
