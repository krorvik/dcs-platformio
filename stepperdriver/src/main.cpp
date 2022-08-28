#define DCSBIOS_DEFAULT_SERIAL
#include "DcsBios.h"
#include "FastAccelStepper.h"
#include "Wire.h"
#include "Button2.h"
#include "ESPRotary.h"
// Stepper data
#define MICROSTEPS 8
// Set this for array sizing
#define STEPPERCOUNT 6
// SWitch/encoder pins
#define SWITCH_PIN  36
#define SDA_PIN     23
#define SCK_PIN    22

// Static setup
// NodeMCU 32s: Fixed pins on PCB, 9 steppers possible:
// Full pinout for ref, STEPPERCOUNT = 9
// const int step_pins[STEPPERCOUNT] = { 32, 25, 27, 12, 21, 18, 17, 4, 2  };
// const int dir_pins[STEPPERCOUNT] =  { 33, 26, 14, 13, 19,  5, 16, 0, 15 };
const int step_pins[STEPPERCOUNT] = { 32, 25, 27, 12, 21, 18 };
const int dir_pins[STEPPERCOUNT] =  { 33, 26, 14, 13, 19,  5 };
const int encoder_pins[2] = { 39, 34 };

// Motor and axis specs
//                                             bank   ptch     oil    noz    rpm    ftit
const int stp_res[STEPPERCOUNT]            = {  200,   720,   720,    720,   720,   720};  // Number of full steps per 360deg rev
const int stp_min[STEPPERCOUNT]            = {    0,     0,     0,      0,     0,     0};  // Minimum position needle (often starting pos)
const int stp_max[STEPPERCOUNT]            = {  630,   630,   630,    630,   630,   630};  // Maximum position needle
const unsigned int stp_speed[STEPPERCOUNT] = { 1200,  1200,  1200,   1200,  1200,  1200};  // Max speed of stepper in steps/sec
const unsigned int stp_accel[STEPPERCOUNT] = {10000, 10000, 10000,  10000, 10000, 10000};  // Accelleration in steps/s^2. 

// Variables
long encoder_position = 0;
// We have one switched encoder, that can adjust all steppers. Swith selects encoder. 
unsigned int adjust_selector = 0; // Can be 0-8, and 9 for off, but we take modulo later for simplicity in switch handler
bool mainPower = false;

// Motor and control objects
FastAccelStepperEngine stepper_engine = FastAccelStepperEngine();
ESPRotary setEncoder(encoder_pins[0], encoder_pins[1]);
Button2 select_button(SWITCH_PIN);
FastAccelStepper *all_steppers[STEPPERCOUNT];

// called by switch callback
void selector_clickhandler(Button2 &button) {
  if (not mainPower) {
    adjust_selector += 1;
  }
}

// called by encoder callback
void adjustStepper(ESPRotary &encoder) {
  if (not mainPower) {
    unsigned int selected_encoder = adjust_selector % 10;  // Will be 0-9 - 9 is off.
    long newPos = encoder.getPosition();
    int diff = encoder_position - newPos;
    if (selected_encoder != 9) {
      all_steppers[selected_encoder]->move(diff);
    }
    encoder_position = newPos;
  }
}

// USed by steppers with zero stop. Unusable for bank/pitch
void setStepperPosition(unsigned int stepper_index, unsigned int newValue) {
  all_steppers[stepper_index]->moveTo(map(newValue, 0, 65535, MICROSTEPS * stp_min[stepper_index], MICROSTEPS * stp_max[stepper_index]));
}

// DCS Bios callback setup, this is where the instruments meet the callbacks
void onSaiBankChange(unsigned int newValue)              { setStepperPosition(0, newValue); }  // This must be replaced with one that handles multiple revolutions. 
void onSaiPitchChange(unsigned int newValue)             { setStepperPosition(1, newValue); }  // This one is actually OK. Adjust stepper limits. 
void onEngineOilPressureChange(unsigned int newValue)    { setStepperPosition(2, newValue); } 
void onEngineNozzlePositionChange(unsigned int newValue) { setStepperPosition(3, newValue); }
void onEngineTachometerChange(unsigned int newValue)     { setStepperPosition(4, newValue); }
void onEngineFtitChange(unsigned int newValue)           { setStepperPosition(5, newValue); }

// This one disables the encoder/switch functions
void onMainPwrSwChange(unsigned int newValue) {
  if(newValue != 0) { mainPower = true;}
}

// SAI and Engine instruments for this one
DcsBios::IntegerBuffer saiBankBuffer(0x44b0, 0xffff, 0, onSaiBankChange);
DcsBios::IntegerBuffer saiPitchBuffer(0x44ae, 0xffff, 0, onSaiPitchChange);
DcsBios::IntegerBuffer engineOilPressureBuffer(0x44da, 0xffff, 0, onEngineOilPressureChange);
DcsBios::IntegerBuffer engineNozzlePositionBuffer(0x44dc, 0xffff, 0, onEngineNozzlePositionChange);
DcsBios::IntegerBuffer engineTachometerBuffer(0x44de, 0xffff, 0, onEngineTachometerChange);
DcsBios::IntegerBuffer engineFtitBuffer(0x44e0, 0xffff, 0, onEngineFtitChange);
DcsBios::IntegerBuffer mainPwrSwBuffer(0x441c, 0x0003, 0, onMainPwrSwChange);

void encoder_init() {
  encoder_position = setEncoder.getPosition();
  setEncoder.setChangedHandler(adjustStepper);
}

void switch_init() {
  select_button.setClickHandler(selector_clickhandler);
}

void steppers_init() {
  stepper_engine.init(0); // Pin stepper engine to ESP32 core 0. DCS bios will run on core 1 (loop).
  // Remember off by one 
  for (int i = 0; i< STEPPERCOUNT; i++) {
    all_steppers[i] = stepper_engine.stepperConnectToPin(step_pins[i]);
    all_steppers[i]->setDirectionPin(dir_pins[i]);
    all_steppers[i]->setSpeedInHz(stp_speed[i] * MICROSTEPS);
    all_steppers[i]->setAcceleration(stp_accel[i]);
    // Spin one revolution backwards, endstopped motors will hit endstop, 
    // others will simply turn one revolution backwards at start. 
    all_steppers[i]->move(-1 * stp_res[i] * MICROSTEPS);
    // All motors start at 0 - but can be adjusted before poweron.
    all_steppers[i]->setCurrentPosition(0);
  }
}

void setup() {
  encoder_init();
  switch_init();
  steppers_init();
  DcsBios::setup();
}

void loop() {
  if (not mainPower) {
    select_button.loop();
    setEncoder.loop();
  }
  DcsBios::loop();
}

