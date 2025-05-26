#define VIPERSTEPPER_H
#include <FastAccelStepper.h>

#define MAXSTEPPERCOUNT 8
#define MICROSTEPS 8

FastAccelStepperEngine stepper_engine = FastAccelStepperEngine();

const int STEP_PINS[MAXSTEPPERCOUNT] =  { 33, 26, 14, 13, 19,  5, 16, 4 };
const int DIR_PINS[MAXSTEPPERCOUNT]  =  { 32, 25, 27, 12, 21, 18, 17, 0 };
unsigned int steppercount = 0;

/*
 * ViperStepper class
 */

class ViperStepper {

  public:
    ViperStepper(        
        unsigned int res, 
        long minval, 
        long maxval, 
        unsigned int speed, 
        unsigned long acceleration
    );
    void move(long);
    void moveTo(unsigned int  steps);
    void reset();
    void wait();

  private:
    FastAccelStepper *stepper;
    unsigned int resolution;
    long min_value;
    long max_value;
    unsigned long acceleration;
    int rotations;    
    long last_position;
    int speed;
};

ViperStepper *all_steppers[MAXSTEPPERCOUNT];

ViperStepper::ViperStepper (     
    unsigned int res, 
    long minval, 
    long maxval, 
    unsigned int speed, 
    unsigned long acceleration
) {
    this->last_position = 0;
    this->rotations = 0;
    this->resolution = res;
    this->min_value = minval;
    this->max_value = maxval;
    this->speed = speed;
    this->stepper = stepper_engine.stepperConnectToPin(STEP_PINS[steppercount]);
    this->stepper->setDirectionPin(DIR_PINS[steppercount]);
    this->stepper->setSpeedInHz(speed * MICROSTEPS);
    this->stepper->setAcceleration(acceleration);
    this->stepper->setCurrentPosition(0);
    // Add to global steppers and count
    all_steppers[steppercount] = this;
    steppercount++;
}

void ViperStepper::wait() {
  while (this->stepper->isRunning()) {};
}

void ViperStepper::reset() {
  this->stepper->stopMove();
  this->wait();
  this->stepper->setCurrentPosition(0);
}

void ViperStepper::move(long steps) {
  // this->stepper->setSpeedInHz(speed * MICROSTEPS);
  this->stepper->move(steps);
}

// Can only move to max and back to min
void ViperStepper::moveTo(unsigned int  position) {
  // this->stepper->setSpeedInHz(speed * MICROSTEPS);
  //rawPosition is 0-65535, and we map to our limits
  this->stepper->moveTo(map(position, 0, 65535, min_value * MICROSTEPS, max_value * MICROSTEPS));
}

/*
 * END ViperStepper class
 */
void stepper_init() {
  stepper_engine.init(0);
}

