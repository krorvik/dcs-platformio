#define DCSBIOS_RS485_SLAVE 88
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"
#include "FastAccelStepper.h"
#include "ESPRotary.h"
#include "Button2.h"

// Controls pins
#define ENCODER_PINA 3
#define ENCODER_PINB 4
#define ENCODER_BUTTON_PIN 5

// Stepper pins
#define PITCH_STEP_PIN 10
#define PITCH_DIR_PIN 12
#define BANK_STEP_PIN 9
#define BANK_DIR_PIN 11

// Stepper max/mins, start pos and speed
#define PITCH_MAX 600
#define PITCH_MIN -600
#define PITCH_DCSBIOS_START 0
#define BANK_MAX 800
#define BANK_MIN -800
#define BANK_DCSBIOS_START 32768
#define STEPPER_SPEED 4000
#define STEPPER_ACCEL 4000

// helper variables
long last_bank_position = 0;
long rawPitch = 32768; // Start centered
long rawBank = 32768;  // Start centered
long bankRotations = 0;
int encoder_position = 0;
unsigned int button_presses = 0;
boolean init_state = true;

FastAccelStepperEngine stepper_engine = FastAccelStepperEngine();
FastAccelStepper *pitch_stepper = NULL;
FastAccelStepper *bank_stepper = NULL;
ESPRotary adjust_encoder(ENCODER_PINA, ENCODER_PINB);
Button2 encoder_button;
FastAccelStepper *stepperselector[2];

int translate_digit(unsigned int value) {
  if (value < 6553) { return 0;}
  if (value < 13107) { return 1;}
  if (value < 19660) { return 2;}
  if (value < 26214) { return 3;}
  if (value < 32767) { return 4;}
  if (value < 39321) { return 5;}
  if (value < 45874) { return 6;}
  if (value < 52428) { return 7;}
  if (value < 58981) { return 8;}
  return 9;
}

void pitch_stepper_init() {
  pitch_stepper = stepper_engine.stepperConnectToPin(PITCH_STEP_PIN);
  stepperselector[0] = pitch_stepper;
  pitch_stepper->setDirectionPin(PITCH_DIR_PIN, false); // false to switch directions
  pitch_stepper->setSpeedInHz(STEPPER_SPEED);
  pitch_stepper->setAcceleration(STEPPER_ACCEL);
  pitch_stepper->setCurrentPosition(PITCH_DCSBIOS_START);
}

void bank_stepper_init() {
  bank_stepper = stepper_engine.stepperConnectToPin(BANK_STEP_PIN);
  stepperselector[1] = bank_stepper;
  bank_stepper->setDirectionPin(BANK_DIR_PIN, false); // false to switch directions
  bank_stepper->setSpeedInHz(STEPPER_SPEED );
  bank_stepper->setAcceleration(STEPPER_ACCEL);
  bank_stepper->setCurrentPosition(BANK_DCSBIOS_START);
}

void adjust_axes(ESPRotary &encoder) {
  unsigned int stepperindex = button_presses % 2;
  FastAccelStepper *stepper = stepperselector[stepperindex];
  long newpos = adjust_encoder.getPosition(); 
  long diff = encoder_position - newpos;
  encoder_position = newpos;
  stepper->move(diff * -2);
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
    bank_stepper->move(100);
    // Sweep forward on exit init, and other direction back
    while (bank_stepper->isRunning()) {}
    bank_stepper->move(-100);
    sendDcsBiosMessage("SAI_CAGE", "0");
  } else {    
    bank_stepper->move(-100);
    while (bank_stepper->isRunning()) {}
    bank_stepper->move(100);
    while (bank_stepper->isRunning()) {}
    // Uncage SAI
    // sendDcsBiosMessage("SAI_CAGE", "1");
    bank_stepper->setCurrentPosition(0);
    pitch_stepper->setCurrentPosition(0);
  }
}

void stepper_init() {
  stepper_engine.init();
  pitch_stepper_init();  
  bank_stepper_init();
}

void onAdiBankChange(unsigned int newValue) {
  // Increasing zero cross?
  if(rawBank > 60535 and newValue < 5000) { bankRotations++; }
  // Decreasing zero cross?
  if(rawBank < 5000 and newValue > 60535) { bankRotations--; }
  rawBank = newValue;
  // Axis inverted, horizon rolls opposite of plane ;) (-1, MAX, MIN) => otherwise (1, MIN, MAX)
  bank_stepper->moveTo(-1 * 1600 * bankRotations + map(rawBank, 0, 65535, BANK_MAX, BANK_MIN));
}

void onAdiPitchChange(unsigned int newValue) {
  rawPitch = newValue;
  if (rawPitch <= 32768) {
    pitch_stepper->moveTo(map(rawPitch, 0, 32768, PITCH_MIN, 0));
  } else {
    pitch_stepper->moveTo(map(rawPitch , 32768, 65535, 0, PITCH_MAX));
  }
}

// DCS buffers
//DcsBios::IntegerBuffer adiBankBuffer            (SAI_BANK_GAUGE_ADDRESS , SAI_BANK_GAUGE_MASK , SAI_BANK_GAUGE_SHIFTBY , onAdiBankChange);
//DcsBios::IntegerBuffer adiPitchBuffer           (SAI_PITCH_GAUGE_ADDRESS , SAI_PITCH_GAUGE_MASK , SAI_PITCH_GAUGE_SHIFTBY , onAdiPitchChange);
DcsBios::IntegerBuffer adiBankBuffer            (ADI_BANK_GAUGE_ADDRESS , ADI_BANK_GAUGE_MASK , ADI_BANK_GAUGE_SHIFTBY , onAdiBankChange);
DcsBios::IntegerBuffer adiPitchBuffer           (ADI_PITCH_GAUGE_ADDRESS , ADI_PITCH_GAUGE_MASK , ADI_PITCH_GAUGE_SHIFTBY , onAdiPitchChange);

void setup() {
  stepper_init();    
  pinMode(ENCODER_BUTTON_PIN, INPUT_PULLUP);
  encoder_button.begin(ENCODER_BUTTON_PIN);
  // Timer seems off, pull down debounce and long click times. 
  // encoder_button.setDebounceTime(25);
  // encoder_button.setLongClickTime(250);
  encoder_button.setPressedHandler(handle_encoder_click);
  encoder_button.setLongClickDetectedHandler(handle_encoder_longpress);
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
