
#define DCSBIOS_RS485_SLAVE 83
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"
#include "helpers.h"
#include "FastAccelStepper.h"
#include "ESPRotary.h"
#include "Button2.h"

#define ENCODER_PINA 3
#define ENCODER_PINB 4
#define ENCODER_BUTTON_PIN 5

// helper variables
int encoder_position = 0;
unsigned int button_presses = 0;
boolean init_state = true;

long alt_100_steps = 0;
long alt_1k_steps = 0;
long alt_10k_steps = 0;

FastAccelStepperEngine stepper_engine = FastAccelStepperEngine();
FastAccelStepper *airspeed_stepper = NULL;
FastAccelStepper *altimeter_stepper = NULL;
ESPRotary altimeter_encoder(ENCODER_PINA, ENCODER_PINB);
Button2 encoder_button; 

void airspeed_stepper_init() {
  airspeed_stepper = stepper_engine.stepperConnectToPin(9);
  airspeed_stepper->setDirectionPin(11);
  airspeed_stepper->setSpeedInHz(6400);
  airspeed_stepper->setAcceleration(6400);
  // Backsweep airspeed to reset
  airspeed_stepper->move(-5760);
  while (airspeed_stepper->isRunning()) {}
  airspeed_stepper->setCurrentPosition(0);
}

void altimeter_stepper_init() {
  altimeter_stepper = stepper_engine.stepperConnectToPin(10);
  altimeter_stepper->setDirectionPin(12);
  altimeter_stepper->setSpeedInHz(6400);
  altimeter_stepper->setAcceleration(6400);
  altimeter_stepper->setCurrentPosition(0);
}

void adjust_altimeter() {
  long newpos = altimeter_encoder.getPosition(); 
  int diff = encoder_position - newpos;
  encoder_position = newpos;
  altimeter_stepper->move(diff * 5);
}

// Short clicks change needle speed when encoder turns
void handle_encoder_click(Button2& b) {
  // Do stuff
  // bump presses last
  button_presses += 1;
}

void handle_encoder_longpress (Button2& b) {
  // Toggle beteeen init and DCS states. 
  init_state = ! init_state;
  if (init_state) {
    // sweep altimeter backwords to initiate init state
    altimeter_stepper->move(-1000);
    while (altimeter_stepper->isRunning()) {}
    delay(1000);
    altimeter_stepper->move(1000);
  } else {
    // Sweep other way to indicate ready
    altimeter_stepper->move(1000);
    while (altimeter_stepper->isRunning()) {}
    delay(1000);
    altimeter_stepper->move(-1000);
    while (altimeter_stepper->isRunning()) {}
    altimeter_stepper->setCurrentPosition(0);
  }
}

void stepper_init() {
  stepper_engine.init();
  airspeed_stepper_init();
  altimeter_stepper_init();
}

void onAirspeedChange(unsigned int newValue)    { airspeed_stepper->moveTo(translate_ias(newValue) * 1.05); } // Scale is slightly off

void onAlt10000FtCntChange(unsigned int newValue) { 
  alt_10k_steps = translate_digit(newValue) * 8 * 720 * 10; // hardcoded res, cause fsck it
}

void onAlt1000FtCntChange(unsigned int newValue)  { 
  alt_1k_steps = translate_digit(newValue) * 8 * 720; // hardcoded res, cause fsck it
}

void onAlt100FtCntChange(unsigned int newValue) {
  alt_100_steps = (long) map(newValue, 0, 65535, 0, 8 * 720);  // hardcoded res, cause fsck it
}

void onUpdateCounterChange(unsigned int newValue) { 
  // stepper->moveTo((int32_t) (alt_100_steps + alt_1k_steps  + alt_10k_steps));
  altimeter_stepper->moveTo(alt_100_steps + alt_1k_steps  + alt_10k_steps);
}

DcsBios::IntegerBuffer airspeedBuffer           (AIRSPEED_GAUGE_ADDRESS , AIRSPEED_GAUGE_MASK , AIRSPEED_GAUGE_SHIFTBY , onAirspeedChange);
DcsBios::IntegerBuffer alt10000FtCntBuffer      (ALT_10000_FT_CNT_GAUGE_ADDRESS , ALT_10000_FT_CNT_GAUGE_MASK , ALT_10000_FT_CNT_GAUGE_SHIFTBY , onAlt10000FtCntChange);
DcsBios::IntegerBuffer alt1000FtCntBuffer       (ALT_1000_FT_CNT_GAUGE_ADDRESS , ALT_1000_FT_CNT_GAUGE_MASK , ALT_1000_FT_CNT_GAUGE_SHIFTBY , onAlt1000FtCntChange);
DcsBios::IntegerBuffer alt100FtCntBuffer        (ALT_100_FT_CNT_GAUGE_ADDRESS , ALT_100_FT_CNT_GAUGE_MASK , ALT_100_FT_CNT_GAUGE_SHIFTBY , onAlt100FtCntChange);
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);

void setup() {
  stepper_init();  
  encoder_button.begin(ENCODER_BUTTON_PIN);
  encoder_button.setClickHandler(handle_encoder_click);
  encoder_button.setLongClickHandler(handle_encoder_longpress);
  altimeter_encoder.setChangedHandler(adjust_altimeter);
  altimeter_encoder.resetPosition();
  DcsBios::setup();
}

void loop() {
  encoder_button.loop();
  if (init_state) {    
    altimeter_encoder.loop();
  } else {
    DcsBios::loop();
  }
}