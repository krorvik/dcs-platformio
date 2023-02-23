#define DCSBIOS_RS485_SLAVE 105
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include <Adafruit_SSD1306.h>
#include "f16c_data.h"
#include "helpers.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C  // 128x32
#define OLED_RESET     -1

char iff_digits[4] = { '-', '-', '-', '-' };

Adafruit_SSD1306 display_iff(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void updateDisplay() {
  
  display_iff.clearDisplay();
  display_iff.dim(true);
  display_iff.setTextSize(4);
  display_iff.setTextColor(WHITE);
  display_iff.setCursor(25, 0);
  display_iff.println(iff_digits);
  display_iff.display();
}

DcsBios::Switch3Pos iffM1Sel1("IFF_M1_SEL_1", 4,3);
DcsBios::Switch3Pos iffM1Sel2("IFF_M1_SEL_2", 6,5);
DcsBios::Switch3Pos iffM3Sel1("IFF_M3_SEL_1", 8,7);
DcsBios::Switch3Pos iffM3Sel2("IFF_M3_SEL_2", 10, 9);

void onIffCodeDrumDigit1Change(unsigned int newValue) { iff_digits[0] = '0' +  translate_digit(newValue); }
void onIffCodeDrumDigit2Change(unsigned int newValue) { iff_digits[1] = '0' +  translate_digit(newValue); }
void onIffCodeDrumDigit3Change(unsigned int newValue) { iff_digits[2] = '0' +  translate_digit(newValue); }
void onIffCodeDrumDigit4Change(unsigned int newValue) { iff_digits[3] = '0' +  translate_digit(newValue); }


DcsBios::IntegerBuffer iffCodeDrumDigit1Buffer(IFF_CODE_DRUM_DIGIT_1_GAUGE_ADDRESS, IFF_CODE_DRUM_DIGIT_1_GAUGE_MASK, IFF_CODE_DRUM_DIGIT_1_GAUGE_SHIFTBY, onIffCodeDrumDigit1Change);
DcsBios::IntegerBuffer iffCodeDrumDigit2Buffer(IFF_CODE_DRUM_DIGIT_2_GAUGE_ADDRESS, IFF_CODE_DRUM_DIGIT_2_GAUGE_MASK, IFF_CODE_DRUM_DIGIT_2_GAUGE_SHIFTBY, onIffCodeDrumDigit2Change);
DcsBios::IntegerBuffer iffCodeDrumDigit3Buffer(IFF_CODE_DRUM_DIGIT_3_GAUGE_ADDRESS, IFF_CODE_DRUM_DIGIT_3_GAUGE_MASK, IFF_CODE_DRUM_DIGIT_3_GAUGE_SHIFTBY, onIffCodeDrumDigit3Change);
DcsBios::IntegerBuffer iffCodeDrumDigit4Buffer(IFF_CODE_DRUM_DIGIT_4_GAUGE_ADDRESS, IFF_CODE_DRUM_DIGIT_4_GAUGE_MASK, IFF_CODE_DRUM_DIGIT_4_GAUGE_SHIFTBY, onIffCodeDrumDigit4Change);

void setup() {
  display_iff.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  updateDisplay();
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
  updateDisplay();
}
