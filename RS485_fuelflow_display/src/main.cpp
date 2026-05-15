#define DCSBIOS_RS485_SLAVE 89
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"
#include "Adafruit_SSD1306.h"

// Display defs
#define DISPLAY_WIDTH 128 // OLED display width, in pixels
#define DISPLAY_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1 
#define DISPLAY_ADDRESS 0x3C
#define ADI_OFF_PIN 10

#define RDY_PIN 3
#define NWS_PIN 5
#define DISC_PIN 6
#define NWS_POT_PIN A0

unsigned int nws_brightness = 255;
unsigned int RDY = 0;
unsigned int NWS = 0;
unsigned int DISC = 0;

char fuelflow_chars[6] = {'0', '0', '0', '0', '0', '\0'};

Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT);

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

void display_fuelflow() {
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(18,5);
  display.println(fuelflow_chars);
  display.display();
}

void flashLEDs() {
  // Flash LEDs
  analogWrite(RDY_PIN, 255);
  analogWrite(NWS_PIN, 255);
  analogWrite(DISC_PIN, 255);
  analogWrite(ADI_OFF_PIN, 64);
  delay(5000);
  analogWrite(DISC_PIN, 0);
  analogWrite(RDY_PIN, 0);
  analogWrite(NWS_PIN, 0);
  analogWrite(ADI_OFF_PIN, 0);
}

// Callback for updating values
void onUpdateCounterChange(unsigned int newValue) {
  analogWrite(RDY_PIN, RDY * nws_brightness);
  analogWrite(NWS_PIN, NWS * nws_brightness);
  analogWrite(DISC_PIN, DISC * nws_brightness);
}

void onAdiGaugeChange(unsigned int newValue) {
  if (newValue < 1000) {
    analogWrite(ADI_OFF_PIN, 0);
  } else {
    analogWrite(ADI_OFF_PIN, 64);
  }
}

// OFF gauge 
// DcsBios::LED adi_off_led (ADI_OFF_FLAG_GAUGE_ADDRESS, ADI_OFF_FLAG_GAUGE_MASK, ADI_OFF_PIN);
DcsBios::Potentiometer arStatusBrtKnb("AR_STATUS_BRT_KNB", NWS_POT_PIN);

// Indexer callbacks
void onLightRdyChange(unsigned int newValue) { RDY = newValue;  }
void onLightArNwsChange(unsigned int newValue) { NWS = newValue;}
void onLightDiscChange(unsigned int newValue) { DISC = newValue;  }
void onArStatusBrtKnbChange(unsigned int newValue) { nws_brightness = map(newValue, 0, 65535, 0, 255); }

// Fuel flow callbacks
void onFuelflowcounter100Change(unsigned int newValue)   { fuelflow_chars[2] = '0' + translate_digit(newValue); }
void onFuelflowcounter1kChange(unsigned int newValue)    { fuelflow_chars[1] = '0' + translate_digit(newValue); }
void onFuelflowcounter10kChange(unsigned int newValue)   { fuelflow_chars[0] = '0' + translate_digit(newValue); }

// DCS buffers
DcsBios::IntegerBuffer adi_off_gauge(SAI_OFF_FLAG_GAUGE_ADDRESS, SAI_OFF_FLAG_GAUGE_MASK, SAI_OFF_FLAG_GAUGE_SHIFTBY, onAdiGaugeChange);
DcsBios::IntegerBuffer fuelflowcounter100Buffer  (FUELFLOWCOUNTER_100_GAUGE_ADDRESS,    FUELFLOWCOUNTER_100_GAUGE_MASK,    FUELFLOWCOUNTER_100_GAUGE_SHIFTBY,    onFuelflowcounter100Change);
DcsBios::IntegerBuffer fuelflowcounter1kBuffer   (FUELFLOWCOUNTER_1K_GAUGE_ADDRESS,     FUELFLOWCOUNTER_1K_GAUGE_MASK,     FUELFLOWCOUNTER_1K_GAUGE_SHIFTBY,     onFuelflowcounter1kChange);
DcsBios::IntegerBuffer fuelflowcounter10kBuffer  (FUELFLOWCOUNTER_10K_GAUGE_ADDRESS,    FUELFLOWCOUNTER_10K_GAUGE_MASK,    FUELFLOWCOUNTER_10K_GAUGE_SHIFTBY,    onFuelflowcounter10kChange);
DcsBios::IntegerBuffer lightRdyBuffer(LIGHT_RDY_LED_ADDRESS, LIGHT_RDY_LED_MASK, LIGHT_RDY_LED_SHIFTBY, onLightRdyChange);
DcsBios::IntegerBuffer lightArNwsBuffer(LIGHT_AR_NWS_LED_ADDRESS, LIGHT_AR_NWS_LED_MASK, LIGHT_AR_NWS_LED_SHIFTBY, onLightArNwsChange);
DcsBios::IntegerBuffer lightDiscBuffer(LIGHT_DISC_LED_ADDRESS, LIGHT_DISC_LED_MASK, LIGHT_DISC_LED_SHIFTBY, onLightDiscChange);
DcsBios::IntegerBuffer ARlightBrightnessBuffer(AIR_REFUEL_LIGHT_KNB_LIMITDIAL_ADDRESS, AIR_REFUEL_LIGHT_KNB_LIMITDIAL_MASK, AIR_REFUEL_LIGHT_KNB_LIMITDIAL_SHIFTBY, onArStatusBrtKnbChange);
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);


void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS);
  display_fuelflow();
  flashLEDs();
  DcsBios::setup();
}
  
void loop() {
    DcsBios::loop();
    display_fuelflow();
}