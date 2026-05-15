#define DCSBIOS_RS485_SLAVE 85
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"
#include "FastAccelStepper.h"


#define AOA_STEPS_OFF 0
#define AOA_STEPS_ZERO 540
#define AOA_STEPS_MAX 1650

#define VVI_STEPS_OFF 0
#define VVI_STEPS_ZERO 880
#define VVI_STEPS_MAX 1600
// Common motors
#define ZERO_SPEED 50
#define SPEED 5000
#define ACCELERATION 10000

// helper variables

FastAccelStepperEngine stepper_engine = FastAccelStepperEngine();
FastAccelStepper *aoa_stepper = NULL;
FastAccelStepper *vvi_stepper = NULL;

unsigned int translate_indexer(unsigned int value, unsigned int steps_min, unsigned int steps_zero, unsigned int steps_max) {
  // TODO: Bad algo. Find real centers, and adjust from there.
  if (value < 32768 and value > 0) {
    return map(value, 0, 32768, steps_min, steps_zero );
  } else if (value >= 32768) {
    return map(value, 32768, 65535, steps_zero, steps_max );
  }
  return 0;
}

unsigned int translate_aoa(unsigned int value) {
  return translate_indexer(value, AOA_STEPS_OFF, AOA_STEPS_ZERO, AOA_STEPS_MAX);
}

unsigned int translate_vvi(unsigned int value) {
  return translate_indexer(value, VVI_STEPS_OFF, VVI_STEPS_ZERO, VVI_STEPS_MAX);
}

void aoa_stepper_init() {
  aoa_stepper = stepper_engine.stepperConnectToPin(10);
  aoa_stepper->setDirectionPin(12, false);
  aoa_stepper->setSpeedInHz(SPEED);
  aoa_stepper->setAcceleration(ACCELERATION);
  aoa_stepper->setCurrentPosition(0);
}

void vvi_stepper_init() {
  vvi_stepper = stepper_engine.stepperConnectToPin(9);
  vvi_stepper->setDirectionPin(11, false);
  vvi_stepper->setSpeedInHz(SPEED);
  vvi_stepper->setAcceleration(ACCELERATION);
  vvi_stepper->setCurrentPosition(0);
}

void stepper_init() {
  stepper_engine.init();
  aoa_stepper_init();  
  vvi_stepper_init();
}

void aoa_wait() {
  while (aoa_stepper->isRunning()) {}
}

void vvi_wait() {
  while (vvi_stepper->isRunning()) {}
}

void wait() {
  vvi_wait();
  aoa_wait();
  delay(2000);
}

void steppers_zero() {
  aoa_stepper->move(-3200);
  vvi_stepper->move(-3200);
  wait();
  aoa_stepper->setSpeedInHz(ZERO_SPEED);
  vvi_stepper->setSpeedInHz(ZERO_SPEED);
  aoa_stepper->move(-300);
  vvi_stepper->move(-300);
  wait();
  aoa_stepper->setCurrentPosition(0);
  vvi_stepper->setCurrentPosition(0);
  aoa_stepper->setSpeedInHz(SPEED);
  vvi_stepper->setSpeedInHz(SPEED);
}

void debug_sweep() {
  aoa_stepper->moveTo(AOA_STEPS_ZERO);
  wait();
  aoa_stepper->moveTo(translate_aoa(32768));
  wait();
  aoa_stepper->moveTo(AOA_STEPS_MAX);
  wait();
  aoa_stepper->moveTo(0);
  wait();
  vvi_stepper->moveTo(VVI_STEPS_ZERO);
  wait();
  vvi_stepper->moveTo(translate_vvi(32768));
  wait();
  vvi_stepper->moveTo(VVI_STEPS_MAX);
  wait();
  vvi_stepper->moveTo(0);
  wait();
}

void onAoaValueChange(unsigned int newValue)    { aoa_stepper->moveTo(translate_aoa(newValue)); }
void onVviChange(unsigned int newValue)         { vvi_stepper->moveTo(translate_vvi(newValue)); }

DcsBios::IntegerBuffer aoaValueBuffer           (AOA_VALUE_GAUGE_ADDRESS , AOA_VALUE_GAUGE_MASK , AOA_VALUE_GAUGE_SHIFTBY , onAoaValueChange);
DcsBios::IntegerBuffer vviBuffer                (VVI_GAUGE_ADDRESS , VVI_GAUGE_MASK , VVI_GAUGE_SHIFTBY , onVviChange);

void setup() {
  stepper_init(); 
  steppers_zero();
  //debug_sweep();
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}


