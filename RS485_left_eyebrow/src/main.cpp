#define DCSBIOS_RS485_SLAVE 3
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"

// AoA indexer
#define RED_PIN 6
#define GREEN_PIN 5
#define YELLOW_PIN 3
#define AOA_POT_PIN A0
#define STARTUP_DELAY_MS 1000

// AoA vars
unsigned int aoa_brightness = 255;
unsigned int RED = 0;
unsigned int GREEN = 0;
unsigned int YELLOW = 0;

// AoA callbacks
void onLightAoaUpChange(unsigned int newValue) { RED = newValue; }
void onLightAoaMidChange(unsigned int newValue) { GREEN = newValue; }
void onLightAoaDnChange(unsigned int newValue) { YELLOW = newValue; }

// Callback for updating values
void onUpdateCounterChange(unsigned int newValue) {
  analogWrite(RED_PIN, RED * aoa_brightness);
  analogWrite(GREEN_PIN, GREEN * aoa_brightness);
  analogWrite(YELLOW_PIN, YELLOW * aoa_brightness);
}

// Hook up pot to refuel dim lever
DcsBios::Potentiometer aoaIndexBrtKnb("AOA_INDEX_BRT_KNB", AOA_POT_PIN);
void onAoaIndexBrtKnbChange(unsigned int newValue) { aoa_brightness = map(newValue, 0, 65535, 0, 255); }

// Read back ar/nws brightness knob value
DcsBios::IntegerBuffer aoaIndexBrtKnbBuffer(AOA_INDEX_BRT_KNB_LIMITDIAL_ADDRESS, AOA_INDEX_BRT_KNB_LIMITDIAL_MASK, AOA_INDEX_BRT_KNB_LIMITDIAL_SHIFTBY, onAoaIndexBrtKnbChange);
DcsBios::IntegerBuffer lightAoaDnBuffer(LIGHT_AOA_DN_LED_ADDRESS, LIGHT_AOA_DN_LED_MASK, LIGHT_AOA_DN_LED_SHIFTBY, onLightAoaDnChange);
DcsBios::IntegerBuffer lightAoaMidBuffer(LIGHT_AOA_MID_LED_ADDRESS, LIGHT_AOA_MID_LED_MASK, LIGHT_AOA_MID_LED_SHIFTBY, onLightAoaMidChange);
DcsBios::IntegerBuffer lightAoaUpBuffer(LIGHT_AOA_UP_LED_ADDRESS, LIGHT_AOA_UP_LED_MASK, LIGHT_AOA_UP_LED_SHIFTBY, onLightAoaUpChange);
DcsBios::Switch2Pos masterCaution("MASTER_CAUTION", 7);
DcsBios::Switch2Pos fAckBtn("F_ACK_BTN", 8);
DcsBios::Switch2Pos iffIdBtn("IFF_ID_BTN", 9);
DcsBios::LED lightMasterCaution4(LIGHT_MASTER_CAUTION_LED_ADDRESS, LIGHT_MASTER_CAUTION_LED_MASK, 10);
DcsBios::LED lightTfFail(LIGHT_TF_FAIL_LED_ADDRESS, LIGHT_TF_FAIL_LED_MASK, 11);
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);

// Very verbose, but easy to tune

void pin_setup() {
  // Modes, start off
  pinMode(RED_PIN, OUTPUT);
  analogWrite(RED_PIN, 0);
  pinMode(GREEN_PIN, OUTPUT);
  analogWrite(GREEN_PIN, 0);
  pinMode(YELLOW_PIN, OUTPUT);
  analogWrite(YELLOW_PIN, 0);
  pinMode(10, OUTPUT);
  digitalWrite(10, 1);
  pinMode(11, OUTPUT);
  digitalWrite(11, 1);
}
 
void pin_flash() {
  // Red
  analogWrite(RED_PIN, aoa_brightness);
  delay(STARTUP_DELAY_MS);
  analogWrite(RED_PIN, 0);
  // Green
  analogWrite(GREEN_PIN, aoa_brightness);
  delay(STARTUP_DELAY_MS);
  analogWrite(GREEN_PIN, 0);
  // Yellow
  analogWrite(YELLOW_PIN, aoa_brightness);
  delay(STARTUP_DELAY_MS);
  analogWrite(YELLOW_PIN, 0);
  // Master caution
  digitalWrite(10, 0);
  delay(STARTUP_DELAY_MS);
  digitalWrite(10, 1);
  // TF FAIL
  digitalWrite(11, 0);
  delay(STARTUP_DELAY_MS);
  digitalWrite(11, 1);
}

void setup() {
  pin_setup();
  pin_flash();
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
