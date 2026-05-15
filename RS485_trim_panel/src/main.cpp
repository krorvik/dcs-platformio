#define DCSBIOS_RS485_SLAVE 65
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Button2.h>

unsigned int yaw = 32768;
unsigned int roll = 32768;
unsigned int pitch = 32768;

// Hardware SPI arduino nano, pin 11 is SDA (MOSI), 13 is SCK
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_CS     9
#define OLED_DC     10
#define OLED_RESET  12

Button2 yaw_reset_button;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS);

// center is at 88,31
void drawPitchRollGrid() {
  for (int i = 0; i < 9; i++) {
    display.drawFastHLine(60, 3 + (i * 7), 4, SSD1306_WHITE);
    display.drawFastHLine(113, 3 + (i * 7), 4, SSD1306_WHITE);
    display.drawFastVLine(60 + (i * 7), 3, 4, SSD1306_WHITE);
    display.drawFastVLine(60 + (i * 7), 56, 4 , SSD1306_WHITE);
  }
  // center lines
  display.drawFastHLine(60, 31, 6, SSD1306_WHITE);
  display.drawFastHLine(111, 31, 6, SSD1306_WHITE);
  display.drawFastVLine(88, 3, 6, SSD1306_WHITE);
  display.drawFastVLine(88, 54, 6 , SSD1306_WHITE);
}

// Center is at 14, 31
void drawYawGrid() {
  for (int i = 0; i < 9; i++) {
    display.drawFastHLine(0, 3 + (i * 7), 4, SSD1306_WHITE);
    display.drawFastHLine(25, 3 + (i * 7), 4, SSD1306_WHITE);
  }
  display.drawFastHLine(0, 31, 6, SSD1306_WHITE);
  display.drawFastHLine(23, 31, 6, SSD1306_WHITE);
}

void drawPitchRollIndicator () {
  unsigned int rollpos_y = map(roll, 0, 65535, 59, 3);
  unsigned int pitchpos_x = map(pitch, 0, 65535, 116, 60);
  display.drawFastHLine(68, rollpos_y, 41, SSD1306_WHITE);
  display.drawFastVLine(pitchpos_x, 11, 41, SSD1306_WHITE);
}

void drawYawIndicator() {
  unsigned int ypos = map(yaw, 0, 65535, 3, 59);
  display.drawFastHLine(10, ypos - 1, 8, SSD1306_WHITE);
  display.drawFastHLine(8, ypos , 12, SSD1306_WHITE);
  display.drawFastHLine(10, ypos + 1 , 8, SSD1306_WHITE);
}

void updateDisplay() {
  display.clearDisplay();
  drawPitchRollGrid();
  drawYawGrid();
  drawPitchRollIndicator();
  drawYawIndicator();
  display.display();
}

void yaw_reset(Button2& button) {
  DcsBios::sendDcsBiosMessage("YAW_TRIM", "32768");
}

// Simple controls
DcsBios::RotaryEncoderT<POLL_EVERY_TIME, DcsBios::FOUR_STEPS_PER_DETENT> pitchTrim("PITCH_TRIM", "-800", "+800", 5, 6);
DcsBios::RotaryEncoderT<POLL_EVERY_TIME, DcsBios::FOUR_STEPS_PER_DETENT> rollTrim("ROLL_TRIM", "-800", "+800", 7, 8);
DcsBios::RotaryEncoderT<POLL_EVERY_TIME, DcsBios::FOUR_STEPS_PER_DETENT> yawTrim("YAW_TRIM", "-800", "+800", 3, 4);

DcsBios::Switch2Pos trimApDiscSw("TRIM_AP_DISC_SW", A0);
// RUD RESET on A1


// callback functions
void onPitchtrimindChange(unsigned int newValue) { pitch = newValue;  }
void onRolltrimindChange(unsigned int newValue)  { roll = newValue; }
void onYawTrimChange(unsigned int newValue) { yaw = newValue; }

// DCS Bios callback defs
DcsBios::IntegerBuffer rolltrimindBuffer(ROLLTRIMIND_GAUGE_ADDRESS, ROLLTRIMIND_GAUGE_MASK, ROLLTRIMIND_GAUGE_SHIFTBY, onRolltrimindChange);
DcsBios::IntegerBuffer pitchtrimindBuffer(PITCHTRIMIND_GAUGE_ADDRESS, PITCHTRIMIND_GAUGE_MASK, PITCHTRIMIND_GAUGE_SHIFTBY, onPitchtrimindChange);
DcsBios::IntegerBuffer yawTrimBuffer(YAW_TRIM_LIMITDIAL_ADDRESS, YAW_TRIM_LIMITDIAL_MASK, YAW_TRIM_LIMITDIAL_SHIFTBY, onYawTrimChange);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  yaw_reset_button.begin(A1);
  yaw_reset_button.setLongClickDetectedHandler(yaw_reset);
  updateDisplay();
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
  yaw_reset_button.loop();
  updateDisplay();
}