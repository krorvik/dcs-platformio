#define DCSBIOS_RS485_SLAVE 85
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"
#include "FastAccelStepper.h"
#include "ESPRotary.h"
#include "Button2.h"

#define ENCODER_PINA 3
#define ENCODER_PINB 4
#define ENCODER_BUTTON_PIN 5

#define AOA_STEPS_MIN 0
#define AOA_STEPS_ZERO 260
#define AOA_STEPS_MAX 880

#define VVI_STEPS_MIN 0
#define VVI_STEPS_ZERO 440
#define VVI_STEPS_MAX 800
// Common motors
#define SPEED 5000 
#define ACCELERATION 4000

// helper variables
int encoder_position = 0;
unsigned int button_presses = 0;
boolean init_state = true;

FastAccelStepperEngine stepper_engine = FastAccelStepperEngine();
FastAccelStepper *aoa_stepper = NULL;
FastAccelStepper *vvi_stepper = NULL;
ESPRotary adjust_encoder(ENCODER_PINA, ENCODER_PINB);
Button2 encoder_button;

FastAccelStepper *stepperselector[2];

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
  return translate_indexer(value, AOA_STEPS_MIN, AOA_STEPS_ZERO, AOA_STEPS_MAX);
}

unsigned int translate_vvi(unsigned int value) {
  return translate_indexer(value, VVI_STEPS_MIN, VVI_STEPS_ZERO, VVI_STEPS_MAX);
}

void aoa_stepper_init() {
  aoa_stepper = stepper_engine.stepperConnectToPin(10);
  stepperselector[0] = aoa_stepper;
  aoa_stepper->setDirectionPin(12);
  aoa_stepper->setSpeedInHz(SPEED);
  aoa_stepper->setAcceleration(ACCELERATION);
  aoa_stepper->setCurrentPosition(0);
}

void vvi_stepper_init() {
  vvi_stepper = stepper_engine.stepperConnectToPin(9);
  stepperselector[1] = vvi_stepper;
  vvi_stepper->setDirectionPin(11);
  vvi_stepper->setSpeedInHz(SPEED);
  vvi_stepper->setAcceleration(ACCELERATION);
  vvi_stepper->setCurrentPosition(0);
}

void adjust_axes() {
  unsigned int stepperindex = button_presses % 2;
  FastAccelStepper *stepper = stepperselector[stepperindex];
  long newpos = adjust_encoder.getPosition(); 
  long diff = encoder_position - newpos;
  encoder_position = newpos;
  stepper->move(diff);
}

// Short clicks change needle speed when encoder turns
void handle_encoder_click(Button2& b) {
  // Do stuff
  // bump presses last
  button_presses += 1;
}

void handle_encoder_longpress (Button2& b) {
  // reset motorpositions
  vvi_stepper->setCurrentPosition(0);
  aoa_stepper->setCurrentPosition(0);
  // Toggle beteeen init and DCS states. 
  init_state = ! init_state;
  // Sweep
  if (init_state) {
    vvi_stepper->moveTo(-200);
    while(vvi_stepper->isRunning()) {}
    // Sweep forward on exit init, and other direction back
    vvi_stepper->moveTo(0);
  } else {
    vvi_stepper->moveTo(200);
    while(vvi_stepper->isRunning()) {}
    vvi_stepper->moveTo(0);    
  }
}

void stepper_init() {
  stepper_engine.init();
  aoa_stepper_init();  
  vvi_stepper_init();
}

void onAoaValueChange(unsigned int newValue)    { aoa_stepper->moveTo(translate_aoa(newValue)); }
void onVviChange(unsigned int newValue)         { vvi_stepper->moveTo(translate_vvi(newValue)); }

DcsBios::IntegerBuffer aoaValueBuffer           (AOA_VALUE_GAUGE_ADDRESS , AOA_VALUE_GAUGE_MASK , AOA_VALUE_GAUGE_SHIFTBY , onAoaValueChange);
DcsBios::IntegerBuffer vviBuffer                (VVI_GAUGE_ADDRESS , VVI_GAUGE_MASK , VVI_GAUGE_SHIFTBY , onVviChange);

void setup() {
  stepper_init();
  encoder_button.begin(ENCODER_BUTTON_PIN);  
  encoder_button.setClickHandler(handle_encoder_click);
  encoder_button.setLongClickHandler(handle_encoder_longpress);
  adjust_encoder.setChangedHandler(adjust_axes);
  adjust_encoder.resetPosition();
  DcsBios::setup();
}

void loop() {
  encoder_button.loop();
  if (init_state) {    
    adjust_encoder.loop();
  } else {
    DcsBios::loop();
  }
}
