#include "FastAccelStepper.h"

FastAccelStepperEngine stepper_engine = FastAccelStepperEngine();
FastAccelStepper *pitch_stepper = NULL;
FastAccelStepper *bank_stepper = NULL;

void pitch_stepper_init() {
  pitch_stepper = stepper_engine.stepperConnectToPin(9);
  pitch_stepper->setDirectionPin(11);
  pitch_stepper->setSpeedInHz(4000);
  pitch_stepper->setAcceleration(4000);
  pitch_stepper->setCurrentPosition(0);
}

void bank_stepper_init() {
  bank_stepper = stepper_engine.stepperConnectToPin(10);

  bank_stepper->setDirectionPin(12);
  bank_stepper->setSpeedInHz(1600);
  bank_stepper->setAcceleration(2000);
  bank_stepper->setCurrentPosition(0);
}

void stepper_init() {
  stepper_engine.init();
  pitch_stepper_init();  
  bank_stepper_init();
}

void setup() {
  stepper_init();    
}
 
void loop() {
  pitch_stepper->runForward();
  bank_stepper->runForward();
}
