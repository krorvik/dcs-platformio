#define DCSBIOS_RS485_SLAVE 75
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"
#include <U8g2lib.h>
#include <Wire.h>

#define DISPLAY_WIDTH 128 // OLED display width, in pixels
#define DISPLAY_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1
#define BUBBLE_MIN 7
#define BUBBLE_MAX 121
#define TURNRATE_MIN 0
#define TURNRATE_MAX 104

#define AUXPIN 3
#define OFFPIN 5
#define LOCPIN 6
#define GSPIN 4

// Limits and center, should be tuned for actual build
const unsigned int dev_xmin = 1;
const unsigned int dev_xmax = 126;
unsigned int deviation = 32768; // Start centered, will be changed by dcs bios callback

U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C deviation_display(U8G2_R2);

// ILS deviation 
void displayDeviation() {
  // Calculate where and what to draw
  unsigned int symbol_position = map(deviation, 0, 65535, dev_xmin, dev_xmax);
  deviation_display.firstPage();
  do {
    // White
    deviation_display.setDrawColor(1);
    // Scale
    deviation_display.drawBox(0,  0, 2, 15);
    deviation_display.drawBox(31, 0, 1, 15);
    deviation_display.drawBox(62, 0, 3, 17);
    deviation_display.drawBox(95, 0, 1, 15);
    deviation_display.drawBox(126,0, 2, 15);
    // Gauge/needle
    deviation_display.drawBox(symbol_position -2, 21, 1, 10);
    deviation_display.drawBox(symbol_position -1, 20, 3, 11);
    deviation_display.drawBox(symbol_position, 19, 1, 1);
    deviation_display.drawBox(symbol_position +2, 21, 1, 10);
  } while ( deviation_display.nextPage() );
}

void onAdiGsPointerChange(unsigned int newValue) {
  deviation = newValue;
  displayDeviation();
}

void setLed(unsigned int pin, unsigned int value) {
  digitalWrite(pin, value > 65534);
};

unsigned int digitalValue(unsigned int value) {
  if (value < 1000) {
    return 0;
  } else {
    return 1;
  }
}

void onAdiAuxGaugeChange(unsigned int newValue) { digitalWrite(AUXPIN, digitalValue(newValue)); }
void onAdiOffGaugeChange(unsigned int newValue) {digitalWrite(OFFPIN, digitalValue(newValue)); }
void onAdiLocGaugeChange(unsigned int newValue) {digitalWrite(LOCPIN, digitalValue(newValue)); }
void onAdiGsGaugeChange(unsigned int newValue) { digitalWrite(GSPIN, digitalValue(newValue)); }

DcsBios::IntegerBuffer adi_aux_buffer           (ADI_AUX_FLAG_GAUGE_ADDRESS, ADI_AUX_FLAG_GAUGE_MASK, ADI_AUX_FLAG_GAUGE_SHIFTBY, onAdiAuxGaugeChange);
DcsBios::IntegerBuffer adi_off_buffer           (ADI_OFF_FLAG_GAUGE_ADDRESS, ADI_OFF_FLAG_GAUGE_MASK, ADI_OFF_FLAG_GAUGE_SHIFTBY, onAdiOffGaugeChange);
DcsBios::IntegerBuffer adi_loc_buffer           (ADI_LOC_FLAG_GAUGE_ADDRESS, ADI_LOC_FLAG_GAUGE_MASK, ADI_LOC_FLAG_GAUGE_SHIFTBY, onAdiLocGaugeChange);
DcsBios::IntegerBuffer adi_gs_buffer            (ADI_GS_FLAG_GAUGE_ADDRESS, ADI_GS_FLAG_GAUGE_MASK, ADI_GS_FLAG_GAUGE_SHIFTBY, onAdiGsGaugeChange);
DcsBios::IntegerBuffer adiGsPointerBuffer       (ADI_GS_POINTER_GAUGE_ADDRESS, ADI_GS_POINTER_GAUGE_MASK, ADI_GS_POINTER_GAUGE_SHIFTBY, onAdiGsPointerChange);

void led_verify() {
  for (int i = 3; i < 7; i++) {
    digitalWrite(i, 1);
  }
  delay(3000);
  // Leave OFF and AUX on for cold starts
  for (int i = 3; i < 7; i++) {
    digitalWrite(i, 0);
  }
}

void setup() {
  deviation_display.begin();
  displayDeviation();
  pinMode(AUXPIN, OUTPUT);
  pinMode(OFFPIN, OUTPUT);
  pinMode(LOCPIN, OUTPUT);
  pinMode(GSPIN, OUTPUT);
  led_verify();
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
