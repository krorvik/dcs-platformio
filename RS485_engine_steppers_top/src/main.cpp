#define DCSBIOS_RS485_SLAVE 73
#define TXENABLE_PIN 2
#include <DcsBios.h>
#include "FastAccelStepper.h"
#include <f16c_data.h>
#include "Adafruit_SSD1306.h"

// Display defs
#define DISPLAY_WIDTH 128 // OLED display width, in pixels
#define DISPLAY_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1 
#define DISPLAY_ADDRESS 0x3C
// Stepper defs, speed and accel are same (VID2x)
#define SPEED 5000 // About a second for a sweep ;)
#define ACCELERATION 65535
// Range of pin in steps, per instrument
#define OIL_STEPS_MAX  4700  // 590 * 8 = 4720
#define NOZ_STEPS_MAX  4000  // 500 * 8 = 4000
// Ardiuno pins - always the same pairs
#define OIL_STEP_PIN 9
#define OIL_DIR_PIN 11
#define NOZ_STEP_PIN 10
#define NOZ_DIR_PIN 12

Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
FastAccelStepperEngine stepper_engine = FastAccelStepperEngine();

FastAccelStepper *oil_stepper = NULL;
FastAccelStepper *noz_stepper = NULL;

char fuelflow_chars[6] = {'0', '0', '0', '0', '0', '\0'};

void stepper_init() {
  stepper_engine.init();
  oil_stepper = stepper_engine.stepperConnectToPin(OIL_STEP_PIN);
  oil_stepper->setDirectionPin(OIL_DIR_PIN);
  oil_stepper->setSpeedInHz(SPEED);
  oil_stepper->setAcceleration(ACCELERATION);
  oil_stepper->setCurrentPosition(0);
  noz_stepper = stepper_engine.stepperConnectToPin(NOZ_STEP_PIN);
  noz_stepper->setDirectionPin(NOZ_DIR_PIN);
  noz_stepper->setSpeedInHz(SPEED);
  noz_stepper->setAcceleration(ACCELERATION);
  noz_stepper->setCurrentPosition(0);
}

void sweep() {
    oil_stepper->moveTo(-5760);
    noz_stepper->moveTo(-5760);
    while(oil_stepper->isRunning()) {}
    while(noz_stepper->isRunning()) {}
    oil_stepper->setCurrentPosition(0);
    noz_stepper->setCurrentPosition(0);
    oil_stepper->moveTo(OIL_STEPS_MAX);
    noz_stepper->moveTo(NOZ_STEPS_MAX);
    while(oil_stepper->isRunning()) {}
    while(noz_stepper->isRunning()) {}
    delay(1000);
    oil_stepper->moveTo(0);
    noz_stepper->moveTo(0);
    while(noz_stepper->isRunning()) {}
    while(oil_stepper->isRunning()) {}
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

void display_fuelflow(unsigned int newValue) {
  display.println(fuelflow_chars);
  display.display();
}

// DCS Bios callback setup, this is where the instruments meet the callbacks - and the steppers are assigned
// TODO: Map from dcs vaslue to steps, map(). 
void onEngineOilPressureChange(unsigned int newValue)    { oil_stepper->moveTo(map(newValue, 0, 65535, 0, OIL_STEPS_MAX)); } 
void onEngineNozzlePositionChange(unsigned int newValue) { noz_stepper->moveTo(map(newValue, 0, 65535, 0, NOZ_STEPS_MAX)); }
void onFuelflowcounter100Change(unsigned int newValue)   { fuelflow_chars[2] = '0' + translate_digit(newValue); }
void onFuelflowcounter1kChange(unsigned int newValue)    { fuelflow_chars[1] = '0' + translate_digit(newValue); }
void onFuelflowcounter10kChange(unsigned int newValue)   { fuelflow_chars[0] = '0' + translate_digit(newValue); }

DcsBios::IntegerBuffer engineOilPressureBuffer   (ENGINE_OIL_PRESSURE_GAUGE_ADDRESS,    ENGINE_OIL_PRESSURE_GAUGE_MASK,    ENGINE_OIL_PRESSURE_GAUGE_SHIFTBY,    onEngineOilPressureChange);
DcsBios::IntegerBuffer engineNozzlePositionBuffer(ENGINE_NOZZLE_POSITION_GAUGE_ADDRESS, ENGINE_NOZZLE_POSITION_GAUGE_MASK, ENGINE_NOZZLE_POSITION_GAUGE_SHIFTBY, onEngineNozzlePositionChange);
DcsBios::IntegerBuffer fuelflowcounter100Buffer  (FUELFLOWCOUNTER_100_GAUGE_ADDRESS,    FUELFLOWCOUNTER_100_GAUGE_MASK,    FUELFLOWCOUNTER_100_GAUGE_SHIFTBY,    onFuelflowcounter100Change);
DcsBios::IntegerBuffer fuelflowcounter1kBuffer   (FUELFLOWCOUNTER_1K_GAUGE_ADDRESS,     FUELFLOWCOUNTER_1K_GAUGE_MASK,     FUELFLOWCOUNTER_1K_GAUGE_SHIFTBY,     onFuelflowcounter1kChange);
DcsBios::IntegerBuffer fuelflowcounter10kBuffer  (FUELFLOWCOUNTER_10K_GAUGE_ADDRESS,    FUELFLOWCOUNTER_10K_GAUGE_MASK,    FUELFLOWCOUNTER_10K_GAUGE_SHIFTBY,    onFuelflowcounter10kChange);
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, display_fuelflow);

void setup() {
  stepper_init();
  sweep();
  display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS);
  display.clearDisplay();   
  display.setTextSize(2);
  display_fuelflow(0);
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}