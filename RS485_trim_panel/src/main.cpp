#define DCSBIOS_RS485_SLAVE 65
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"
#include <Adafruit_SSD1306.h>
//#include <U8g2lib.h>
#include <Wire.h>

#define DISPLAY_WIDTH 128 // OLED display width, in pixels
#define DISPLAY_HEIGHT 32 // OLED display height, in pixels
#define DISPLAY_ADDRESS 0x3C
#define MUX_ADDRESS 0x70
#define OLED_RESET     -1

Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, OLED_RESET);

// Simple controls
DcsBios::RotaryEncoderT<POLL_EVERY_TIME, DcsBios::FOUR_STEPS_PER_DETENT> pitchTrim("PITCH_TRIM", "-1600", "+1600", 6,5);
DcsBios::RotaryEncoderT<POLL_EVERY_TIME, DcsBios::FOUR_STEPS_PER_DETENT>  rollTrim("ROLL_TRIM", "-1600", "+1600", 4,3);
DcsBios::Switch2Pos trimApDiscSw("TRIM_AP_DISC_SW", 7);
DcsBios::Potentiometer yawTrim("YAW_TRIM", A0);

// Switch the bus on the tmux - used if we request a bus in 
void switchBus(uint8_t busid) {
  Wire.beginTransmission(MUX_ADDRESS);
  Wire.write(1 << busid);
  Wire.endTransmission();
}

void drawScale(unsigned int mux_addr, unsigned int value) {
  // draw scale on requested disåplay
  switchBus(mux_addr);  
  display.clearDisplay();
  display.dim(true);
  // display.dim(true);
  // Lines at first end
  display.drawFastVLine(3,0 , 12, SSD1306_WHITE);
  display.drawFastVLine(32,0 , 6, SSD1306_WHITE);
  // thicker center line
  display.drawFastVLine(63,0 , 12, SSD1306_WHITE);
  display.drawFastVLine(64,0 , 14, SSD1306_WHITE);
  display.drawFastVLine(65,0 , 12, SSD1306_WHITE);
  // Lines at other end
  display.drawFastVLine(96,0 , 6, SSD1306_WHITE);
  display.drawFastVLine(125,0 , 12, SSD1306_WHITE);

  // Caret/arrow from value
  unsigned int position = map(value, 0, 65535, 3, 125);
  display.drawFastVLine(position - 5 ,28 ,  4, SSD1306_WHITE);
  display.drawFastVLine(position - 4 ,26 ,  6, SSD1306_WHITE);
  display.drawFastVLine(position - 3 ,24 ,  8, SSD1306_WHITE);
  display.drawFastVLine(position - 2 ,22 , 10, SSD1306_WHITE);
  display.drawFastVLine(position - 1 ,20 , 12, SSD1306_WHITE);
  display.drawFastVLine(position     ,18 , 14, SSD1306_WHITE);
  display.drawFastVLine(position + 1 ,20 , 12, SSD1306_WHITE);
  display.drawFastVLine(position + 2 ,22 , 10, SSD1306_WHITE);
  display.drawFastVLine(position + 3 ,24 ,  8, SSD1306_WHITE);
  display.drawFastVLine(position + 4 ,26 ,  6, SSD1306_WHITE);
  display.drawFastVLine(position + 5 ,28 ,  4, SSD1306_WHITE);

  display.display();
}

// callback functions
void onPitchtrimindChange(unsigned int newValue) { drawScale(2, newValue); }
void onRolltrimindChange(unsigned int newValue)  { drawScale(1, newValue); }

// DCS Bios callback defs
DcsBios::IntegerBuffer rolltrimindBuffer(ROLLTRIMIND_GAUGE_ADDRESS, ROLLTRIMIND_GAUGE_MASK, ROLLTRIMIND_GAUGE_SHIFTBY, onRolltrimindChange);
DcsBios::IntegerBuffer pitchtrimindBuffer(PITCHTRIMIND_GAUGE_ADDRESS, PITCHTRIMIND_GAUGE_MASK, PITCHTRIMIND_GAUGE_SHIFTBY, onPitchtrimindChange);

void setup() {  
  DcsBios::setup();
  // Center trim indicators
  // Init displays
  Wire.begin();
  switchBus(1); 
  display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS);
  switchBus(2); 
  display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS);
  // Draw initial
  drawScale(1, 32768);
  drawScale(2, 32768);
}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}
