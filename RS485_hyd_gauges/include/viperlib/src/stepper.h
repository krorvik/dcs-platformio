#ifndef VIPERSTEPPER_H
#define VIPERSTEPPER_H
#include <DcsBios.h>
#include <FastAccelStepper.h>

#define MAXSTEPPERCOUNT 8
#define SWITCH_PIN 2  // Bottom right iopin (center con) https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fesphome.io%2F_images%2Fnodemcu_esp32-full.jpg&f=1&nofb=1
#define PWM_PIN 15      // inner con is PWM input
#define SMOOTH 5
#define PWM_MAX 253 //Slight deadzone
#define MICROSTEPS 8
#define PULSEIN_TIMEOUT 5000
#define ZEROMARGIN 6000 

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
        unsigned int acceleration,
        unsigned int adjustfactor
    );
    void moveToBounded(long position);
    void moveToContinuous(long position);
    void move(long steps);
    void moveTo(long steps);
    void reset();
    void wait();
    unsigned int adjustFactor;

  private:
    FastAccelStepper *stepper;
    unsigned int resolution;
    long min_value;
    long max_value;
    unsigned int acceleration;
    int rotations;    
    long last_position;
};

ViperStepper *all_steppers[MAXSTEPPERCOUNT];

ViperStepper::ViperStepper (     
    unsigned int res, 
    long minval, 
    long maxval, 
    unsigned int speed, 
    unsigned int acceleration,
    unsigned int adjustfactor
) {
    this->last_position = 0;
    this->rotations = 0;
    this->resolution = res;
    this->min_value = minval;
    this->max_value = maxval;
    this->adjustFactor = adjustfactor;
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

// Can only move to max and back to min
void ViperStepper::moveToBounded(long position) {
  //rawPosition is 0-65535, and we map to our limits
  this->stepper->moveTo(map(position, 0, 65535, min_value * MICROSTEPS, max_value * MICROSTEPS));
}

void ViperStepper::moveTo(long steps) {
    this->stepper->moveTo(steps);
}

void ViperStepper::move(long steps) {
    this->stepper->move(steps);
}

/*
 * END ViperStepper class
 */
void stepper_init() {
  stepper_engine.init(0); 
  pinMode(PWM_PIN, INPUT);
}

#endif //VIPERSTEPPER_H

