#define DCSBIOS_RS485_SLAVE 84
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"
#include "FastAccelStepper.h"
#include "ESPRotary.h"
#include "Button2.h"

#define ENCODER_PINA 3
#define ENCODER_PINB 4
#define ENCODER_BUTTON_PIN 5
#define PITCH_MAX 5400
#define PITCH_MIN -5500

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

void pitch_stepper_init() {
  pitch_stepper = stepper_engine.stepperConnectToPin(9);
  stepperselector[0] = pitch_stepper;
  pitch_stepper->setDirectionPin(11);
  pitch_stepper->setSpeedInHz(4000);
  pitch_stepper->setAcceleration(4000);
  pitch_stepper->setCurrentPosition(0);
}

void bank_stepper_init() {
  bank_stepper = stepper_engine.stepperConnectToPin(10);
  stepperselector[1] = bank_stepper;
  bank_stepper->setDirectionPin(12);
  bank_stepper->setSpeedInHz(1600);
  bank_stepper->setAcceleration(2000);
  bank_stepper->setCurrentPosition(32768);
}

void adjust_axes() {
  unsigned int stepperindex = button_presses % 2;
  FastAccelStepper *stepper = stepperselector[stepperindex];
  int multiplier = 5;
  // Move more if we're pitch stepper
  if (stepperindex == 0) {
    multiplier = 25;
  }
  long newpos = adjust_encoder.getPosition(); 
  long diff = encoder_position - newpos;
  encoder_position = newpos;
  stepper->move(diff * multiplier);
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
    delay(1000);
    bank_stepper->move(-100);
  } else {
    bank_stepper->move(-100);
    while (bank_stepper->isRunning()) {}
    delay(1000);
    bank_stepper->move(100);
    while (bank_stepper->isRunning()) {}
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
  bank_stepper->moveTo(-1 * 1600 * bankRotations + map(rawBank, 0, 65535, 800, -800));
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
DcsBios::IntegerBuffer adiBankBuffer            (ADI_BANK_GAUGE_ADDRESS , ADI_BANK_GAUGE_MASK , ADI_BANK_GAUGE_SHIFTBY , onAdiBankChange);
DcsBios::IntegerBuffer adiPitchBuffer           (ADI_PITCH_GAUGE_ADDRESS , ADI_PITCH_GAUGE_MASK , ADI_PITCH_GAUGE_SHIFTBY , onAdiPitchChange);

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
