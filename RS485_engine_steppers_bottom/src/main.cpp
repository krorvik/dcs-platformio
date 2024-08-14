#define DCSBIOS_RS485_SLAVE 72
#define TXENABLE_PIN 2
#include <DcsBios.h>
#include "FastAccelStepper.h"
#include <f16c_data.h>

// Stepper defs, speed and accel are same (VID2x)
#define SPEED 5000 // About a second for a sweep ;)
#define ACCELERATION 65535
// Range of pin in steps, per instrument
#define RPM_STEPS_MAX  4960  // 590 * 8 = 4720
#define FTIT_STEPS_MAX  5360  // 500 * 8 = 4000
// Ardiuno pins - always the same pairs
#define RPM_STEP_PIN 9
#define RPM_DIR_PIN 11
#define FTIT_STEP_PIN 10
#define FTIT_DIR_PIN 12

FastAccelStepperEngine stepper_engine = FastAccelStepperEngine();

FastAccelStepper *rpm_stepper = NULL;
FastAccelStepper *ftit_stepper = NULL;

void stepper_init() {
  stepper_engine.init();
  rpm_stepper = stepper_engine.stepperConnectToPin(RPM_STEP_PIN);
  rpm_stepper->setDirectionPin(RPM_DIR_PIN);
  rpm_stepper->setSpeedInHz(SPEED);
  rpm_stepper->setAcceleration(ACCELERATION);
  rpm_stepper->setCurrentPosition(0);
  ftit_stepper = stepper_engine.stepperConnectToPin(FTIT_STEP_PIN);
  ftit_stepper->setDirectionPin(FTIT_DIR_PIN);
  ftit_stepper->setSpeedInHz(SPEED);
  ftit_stepper->setAcceleration(ACCELERATION);
  ftit_stepper->setCurrentPosition(0);
}

void sweep() {
    rpm_stepper->moveTo(-5760);
    ftit_stepper->moveTo(-5760);
    while(rpm_stepper->isRunning()) {}
    while(ftit_stepper->isRunning()) {}
    rpm_stepper->setCurrentPosition(0);
    ftit_stepper->setCurrentPosition(0);
    rpm_stepper->moveTo(RPM_STEPS_MAX);
    ftit_stepper->moveTo(FTIT_STEPS_MAX);
    while(rpm_stepper->isRunning()) {}
    while(ftit_stepper->isRunning()) {}
    delay(1000);
    rpm_stepper->moveTo(0);
    ftit_stepper->moveTo(0);
    while(ftit_stepper->isRunning()) {}
    while(rpm_stepper->isRunning()) {}
}

// DCS Bios callback setup, this is where the instruments meet the callbacks - and the steppers are assigned
// TODO: Map from dcs vaslue to steps, map(). 
void onEngineRPMChange(unsigned int newValue)    { rpm_stepper->moveTo(map(newValue, 0, 65535, 0, RPM_STEPS_MAX)); } 
void onEngineFTITChange(unsigned int newValue) { ftit_stepper->moveTo(map(newValue, 0, 65535, 0, FTIT_STEPS_MAX)); }
DcsBios::IntegerBuffer engineOilPressureBuffer   (ENGINE_TACHOMETER_GAUGE_ADDRESS,    ENGINE_TACHOMETER_GAUGE_MASK,    ENGINE_TACHOMETER_GAUGE_SHIFTBY,  onEngineRPMChange);
DcsBios::IntegerBuffer engineNozzlePositionBuffer(ENGINE_FTIT_GAUGE_ADDRESS, ENGINE_FTIT_GAUGE_MASK, ENGINE_FTIT_GAUGE_SHIFTBY, onEngineFTITChange);

void setup() {
  stepper_init();
  sweep();  
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}