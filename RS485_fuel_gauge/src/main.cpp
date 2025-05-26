#define DCSBIOS_RS485_SLAVE 47
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"
#include "FastAccelStepper.h"

// Stepper defs, speed and accel are same (VID2x)
#define SPEED 5000 // About a second for a sweep ;)
#define ACCELERATION 65535
// Ardiuno pins - always the same pairs
#define STEP_PIN_A 9
#define DIR_PIN_A 11
#define STEP_PIN_B 10
#define DIR_PIN_B 12
// Range of stepper in steps, 360 and range of gauge
#define STEPS_MAX 5760
#define STEPS_MAX_A  4960 
#define STEPS_MAX_B  4960 

// Steppers
FastAccelStepperEngine stepper_engine = FastAccelStepperEngine();
FastAccelStepper *stepper_a = NULL;
FastAccelStepper *stepper_b = NULL;

// Stepper functions
void stepper_init() {
  stepper_engine.init();
  stepper_a = stepper_engine.stepperConnectToPin(STEP_PIN_A);
  stepper_a->setDirectionPin(DIR_PIN_A, false); // false switches direction
  stepper_a->setSpeedInHz(SPEED);
  stepper_a->setAcceleration(ACCELERATION);
  stepper_a->setCurrentPosition(0);
  stepper_b = stepper_engine.stepperConnectToPin(STEP_PIN_B);
  stepper_b->setDirectionPin(DIR_PIN_B, false);
  stepper_b->setSpeedInHz(SPEED);
  stepper_b->setAcceleration(ACCELERATION);
  stepper_b->setCurrentPosition(0);
}

void steppers_sweep() {
    stepper_a->moveTo(-5760);
    stepper_b->moveTo(-5760);
    while(stepper_a->isRunning()) {}
    while(stepper_b->isRunning()) {}
    stepper_a->setCurrentPosition(0);
    stepper_b->setCurrentPosition(0);
    stepper_a->moveTo(STEPS_MAX_A);
    stepper_b->moveTo(STEPS_MAX_B);
    while(stepper_a->isRunning()) {}
    while(stepper_b->isRunning()) {}
    delay(1000);
    stepper_a->moveTo(0);
    stepper_b->moveTo(0);
    while(stepper_b->isRunning()) {}
    while(stepper_a->isRunning()) {}
}

// callback functions, move steppers ;)
void onFuelFrChange(unsigned int newValue)           { stepper_a->moveTo(map(newValue, 0, 65535, 0, STEPS_MAX_A)); }
void onFuelAlChange(unsigned int newValue)           { stepper_b->moveTo(map(newValue, 0, 65535, 0, STEPS_MAX_B)); }

// DCS Bios callback setup, this is where the instruments meet the callbacks - and the steppers are assigned
DcsBios::IntegerBuffer fuelFrBuffer(FUEL_FR_GAUGE_ADDRESS , FUEL_FR_GAUGE_MASK , FUEL_FR_GAUGE_SHIFTBY , onFuelFrChange);
DcsBios::IntegerBuffer fuelAlBuffer(FUEL_AL_GAUGE_ADDRESS , FUEL_AL_GAUGE_MASK , FUEL_AL_GAUGE_SHIFTBY , onFuelAlChange);

// Arduino standard functions
void setup() {
    
    stepper_init();
    steppers_sweep();
    DcsBios::setup();
}

void loop() {
    DcsBios::loop();
}