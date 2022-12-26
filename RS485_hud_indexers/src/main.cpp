#define DCSBIOS_RS485_SLAVE 3
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"

// Refuel/NWS indexer
#define RDY_PIN 3
#define NWS_PIN 5
#define DISC_PIN 6
#define NWS_POT_PIN A0
// AoA indexer
#define RED_PIN 9
#define GREEN_PIN 10
#define YELLOW_PIN 11
#define AOA_POT_PIN A1

// NWS vars
unsigned int nws_brightness = 255;
unsigned int RDY = 0;
unsigned int NWS = 0;
unsigned int DISC = 0;
// AoA vars
unsigned int aoa_brightness = 255;
unsigned int RED = 0;
unsigned int GREEN = 0;
unsigned int YELLOW = 0;


// NWS callbacks
void onLightRdyChange(unsigned int newValue) { RDY = newValue; }
void onLightArNwsChange(unsigned int newValue) { NWS = newValue; }
void onLightDiscChange(unsigned int newValue) { DISC = newValue; }
// AoA callbacks
void onLightAoaUpChange(unsigned int newValue) { RED = newValue; }
void onLightAoaMidChange(unsigned int newValue) { GREEN = newValue; }
void onLightAoaDnChange(unsigned int newValue) { YELLOW = newValue; }

// Callback for updating values
void onUpdateCounterChange(unsigned int newValue) {
  analogWrite(RDY_PIN, RDY * nws_brightness);
  analogWrite(NWS_PIN, NWS * nws_brightness);
  analogWrite(DISC_PIN, DISC * nws_brightness);
  analogWrite(RED_PIN, RED * aoa_brightness);
  analogWrite(GREEN_PIN, GREEN * aoa_brightness);
  analogWrite(YELLOW_PIN, YELLOW * aoa_brightness);
}

// Hook up pot to refuel dim lever
DcsBios::Potentiometer arStatusBrtKnb("AR_STATUS_BRT_KNB", NWS_POT_PIN);
DcsBios::Potentiometer aoaIndexBrtKnb("AOA_INDEX_BRT_KNB", AOA_POT_PIN);

void onArStatusBrtKnbChange(unsigned int newValue) { nws_brightness = map(newValue, 0, 65535, 0, 255); }
void onAoaIndexBrtKnbChange(unsigned int newValue) { aoa_brightness = map(newValue, 0, 65535, 0, 255); }

// Read back ar/nws brightness knob value
DcsBios::IntegerBuffer arStatusBrtKnbBuffer(AR_STATUS_BRT_KNB_LIMITDIAL_ADDRESS, AR_STATUS_BRT_KNB_LIMITDIAL_MASK, AR_STATUS_BRT_KNB_LIMITDIAL_SHIFTBY, onArStatusBrtKnbChange);
DcsBios::IntegerBuffer aoaIndexBrtKnbBuffer(AOA_INDEX_BRT_KNB_LIMITDIAL_ADDRESS, AOA_INDEX_BRT_KNB_LIMITDIAL_MASK, AOA_INDEX_BRT_KNB_LIMITDIAL_SHIFTBY, onAoaIndexBrtKnbChange);
DcsBios::IntegerBuffer lightRdyBuffer(LIGHT_RDY_LED_ADDRESS, LIGHT_RDY_LED_MASK, LIGHT_RDY_LED_SHIFTBY, onLightRdyChange);
DcsBios::IntegerBuffer lightArNwsBuffer(LIGHT_AR_NWS_LED_ADDRESS, LIGHT_AR_NWS_LED_MASK, LIGHT_AR_NWS_LED_SHIFTBY, onLightArNwsChange);
DcsBios::IntegerBuffer lightDiscBuffer(LIGHT_DISC_LED_ADDRESS, LIGHT_DISC_LED_MASK, LIGHT_DISC_LED_SHIFTBY, onLightDiscChange);
DcsBios::IntegerBuffer lightAoaDnBuffer(LIGHT_AOA_DN_LED_ADDRESS, LIGHT_AOA_DN_LED_MASK, LIGHT_AOA_DN_LED_SHIFTBY, onLightAoaDnChange);
DcsBios::IntegerBuffer lightAoaMidBuffer(LIGHT_AOA_MID_LED_ADDRESS, LIGHT_AOA_MID_LED_MASK, LIGHT_AOA_MID_LED_SHIFTBY, onLightAoaMidChange);
DcsBios::IntegerBuffer lightAoaUpBuffer(LIGHT_AOA_UP_LED_ADDRESS, LIGHT_AOA_UP_LED_MASK, LIGHT_AOA_UP_LED_SHIFTBY, onLightAoaUpChange);
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);

void pin_setup() {
  analogWrite(RDY_PIN, nws_brightness);
  analogWrite(DISC_PIN, nws_brightness);
  analogWrite(NWS_PIN, nws_brightness);
  analogWrite(RED_PIN, aoa_brightness);
  analogWrite(YELLOW_PIN, aoa_brightness);
  analogWrite(GREEN_PIN, aoa_brightness);
  delay(2000);
  analogWrite(DISC_PIN, 0);
  analogWrite(RDY_PIN, 0);
  analogWrite(NWS_PIN, 0);
  analogWrite(YELLOW_PIN, 0);
  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 0);
}

void setup() {
  pin_setup();
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
