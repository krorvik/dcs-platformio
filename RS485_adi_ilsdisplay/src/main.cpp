#define DCSBIOS_RS485_SLAVE 76
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C turnindicator_display(U8G2_R0);

// Limits and center, should be tuned for actual build
const int dev_xmin = -6;
const int dev_xcenter = 56;
const int dev_xmax = 118;
const int ball_min = 7;
const int ball_max = 121;
unsigned int ball_pos = 64;

void draw_turnindicator() {
  turnindicator_display.firstPage();
  do {
    // Static pattern
    turnindicator_display.setDrawColor(1);
    turnindicator_display.drawBox(0,0, 128, 18);
    turnindicator_display.drawBox(2, 19, 24, 6);
    turnindicator_display.drawBox(52, 19, 24, 6);
    turnindicator_display.drawBox(104, 19, 24, 6);
    
    // Draw sideslip box
    turnindicator_display.drawBox(52, 26, 24, 6);
    
    // Set drawColor to 0, 1 is default - invert color for ball
    turnindicator_display.setDrawColor(0);
    // Static for ball
    turnindicator_display.drawLine(55, 0, 55, 18);
    turnindicator_display.drawLine(73, 0, 73, 18);

    // Actual ball, must change to dynamic when we have buffers 
    turnindicator_display.drawDisc(ball_pos, 9,7);
    
  } while ( turnindicator_display.nextPage() );
}

void onGsChange(unsigned int value) {
  ball_pos = map(value, 0, 65535, ball_min, ball_max);
}

void onUpdateCounterChange(unsigned int newValue) {
  draw_turnindicator();
}

DcsBios::IntegerBuffer devBuffer(ADI_GS_BAR_GAUGE_ADDRESS, ADI_GS_BAR_GAUGE_MASK, ADI_GS_BAR_GAUGE_SHIFTBY, onGsChange);
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);

void setup() {
  turnindicator_display.begin();
  draw_turnindicator();
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
