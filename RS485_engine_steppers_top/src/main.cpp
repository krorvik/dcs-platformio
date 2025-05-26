#define DCSBIOS_RS485_SLAVE 73
#define TXENABLE_PIN 2
#include <DcsBios.h>
#include "FastAccelStepper.h"
#include <f16c_data.h>

// Stepper defs, speed and accel are same (VID2x)
#define SPEED 5000 // About a second for a sweep ;)
#define ACCELERATION 65535
// Range of pin in steps, per instrument
#define OIL_STEPS_MAX  4700  // 590 * 8 = 4720
#define NOZ_STEPS_MAX  4000  // 500 * 8 = 4000
// Ardiuno pins - always the same pairs
#define OIL_STEP_PIN 10
#define OIL_DIR_PIN 12
#define NOZ_STEP_PIN 9
#define NOZ_DIR_PIN 11

FastAccelStepperEngine stepper_engine = FastAccelStepperEngine();
FastAccelStepper *oil_stepper = NULL;
FastAccelStepper *noz_stepper = NULL;

void stepper_init() {
  stepper_engine.init();
  oil_stepper = stepper_engine.stepperConnectToPin(OIL_STEP_PIN);
  oil_stepper->setDirectionPin(OIL_DIR_PIN, true);
  oil_stepper->setSpeedInHz(SPEED);
  oil_stepper->setAcceleration(ACCELERATION);
  oil_stepper->setCurrentPosition(0);
  noz_stepper = stepper_engine.stepperConnectToPin(NOZ_STEP_PIN);
  noz_stepper->setDirectionPin(NOZ_DIR_PIN, false);
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

// DCS Bios callback setup, this is where the instruments meet the callbacks - and the steppers are assigned
void onEngineOilPressureChange(unsigned int newValue)    { oil_stepper->moveTo(map(newValue, 0, 65535, 0, OIL_STEPS_MAX)); } 
void onEngineNozzlePositionChange(unsigned int newValue) { noz_stepper->moveTo(map(newValue, 0, 65535, 0, NOZ_STEPS_MAX)); }

DcsBios::IntegerBuffer engineOilPressureBuffer   (ENGINE_OIL_PRESSURE_GAUGE_ADDRESS,    ENGINE_OIL_PRESSURE_GAUGE_MASK,    ENGINE_OIL_PRESSURE_GAUGE_SHIFTBY,    onEngineOilPressureChange);
DcsBios::IntegerBuffer engineNozzlePositionBuffer(ENGINE_NOZZLE_POSITION_GAUGE_ADDRESS, ENGINE_NOZZLE_POSITION_GAUGE_MASK, ENGINE_NOZZLE_POSITION_GAUGE_SHIFTBY, onEngineNozzlePositionChange);

void setup() {
  stepper_init();
  sweep();  
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}