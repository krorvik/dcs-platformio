#define DCSBIOS_RS485_SLAVE 47
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"
#include "FastAccelStepper.h"
#include <Adafruit_SSD1306.h>

// Totalizer defs
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C  // 128x32
#define OLED_RESET     -1

#define INIT_SPEED 250
#define SPEED 1000
#define ACCELERATION 500
#define INIT_ACCELERATION 100
// new gauge 120. Old gauge 0, individual by design. 
#define ZERO_OFFSET 120

// Ardiuno pins - always the same pairs
#define STEP_PIN_A 10
#define DIR_PIN_A 12
#define STEP_PIN_B 9
#define DIR_PIN_B 11
// Range of stepper in steps, 360 and range of gauge
#define STEPS_MAX 5760
#define STEPS_MAX_A  5050 
#define STEPS_MAX_B  5050 

char fuel_totalizer[6] = {'0','0','0','0','0', '\0'};

// Display
Adafruit_SSD1306 display_totalizer(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Steppers
FastAccelStepperEngine stepper_engine = FastAccelStepperEngine();
FastAccelStepper *stepper_a = NULL;
FastAccelStepper *stepper_b = NULL;

// Display funcs
void displayFuel() {
  display_totalizer.clearDisplay();
  display_totalizer.setTextSize(3);
  display_totalizer.setTextColor(WHITE);
  display_totalizer.setCursor(12, 0);
  display_totalizer.println(fuel_totalizer);
  display_totalizer.display();
}

// Stepper functions
void stepper_init() {
  stepper_engine.init();
  stepper_a = stepper_engine.stepperConnectToPin(STEP_PIN_A);
  stepper_a->setDirectionPin(DIR_PIN_A, false); // false switches direction
  stepper_a->setSpeedInHz(INIT_SPEED);
  stepper_a->setAcceleration(INIT_ACCELERATION);
  stepper_a->setCurrentPosition(0);
  stepper_b = stepper_engine.stepperConnectToPin(STEP_PIN_B);
  stepper_b->setDirectionPin(DIR_PIN_B, false);
  stepper_b->setSpeedInHz(INIT_SPEED);
  stepper_b->setAcceleration(INIT_ACCELERATION);
  stepper_b->setCurrentPosition(0);
}

void steppers_sweep() {
    // one needle at a time
    stepper_a->moveTo(-1 * STEPS_MAX);
    stepper_a->setSpeedInHz(SPEED);
    stepper_a->setAcceleration(ACCELERATION);
    while(stepper_a->isRunning()) {}
    stepper_a->setCurrentPosition(0);
    stepper_a->moveTo(STEPS_MAX_A);
    while(stepper_a->isRunning()) {}
    delay(1000);
    stepper_a->moveTo(ZERO_OFFSET);
    while(stepper_a->isRunning()) {}

    stepper_b->moveTo(-1 * STEPS_MAX);
    stepper_b->setSpeedInHz(SPEED);
    stepper_b->setAcceleration(ACCELERATION);
    while(stepper_b->isRunning()) {}
    stepper_b->setCurrentPosition(0);
    stepper_b->moveTo(STEPS_MAX_B);
    while(stepper_b->isRunning()) {}
    delay(1000);
    stepper_b->moveTo(ZERO_OFFSET);
    while(stepper_b->isRunning()) {}
}

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

// callback functions, move steppers ;)
void onFuelFrChange(unsigned int newValue)           { stepper_a->moveTo(map(newValue, 0, 65535, ZERO_OFFSET, STEPS_MAX_A)); }
void onFuelAlChange(unsigned int newValue)           { stepper_b->moveTo(map(newValue, 0, 65535, ZERO_OFFSET, STEPS_MAX_B)); }
void onFueltotalizer100Change(unsigned int newValue) {fuel_totalizer[2] = '0' + translate_digit(newValue) ;}
void onFueltotalizer1kChange(unsigned int newValue) {fuel_totalizer[1] = '0' + translate_digit(newValue) ;}
void onFueltotalizer10kChange(unsigned int newValue) {fuel_totalizer[0] = '0' + translate_digit(newValue) ;}

void onUpdateCounterChange(unsigned int newValue) {   
  displayFuel(); 
}

// DCS Bios callback setup, this is where the instruments meet the callbacks - and the steppers are assigned
DcsBios::IntegerBuffer fuelFrBuffer(FUEL_FR_GAUGE_ADDRESS , FUEL_FR_GAUGE_MASK , FUEL_FR_GAUGE_SHIFTBY , onFuelFrChange);
DcsBios::IntegerBuffer fuelAlBuffer(FUEL_AL_GAUGE_ADDRESS , FUEL_AL_GAUGE_MASK , FUEL_AL_GAUGE_SHIFTBY , onFuelAlChange);
DcsBios::IntegerBuffer fueltotalizer100Buffer(FUELTOTALIZER_100_GAUGE_ADDRESS, FUELTOTALIZER_100_GAUGE_MASK, FUELTOTALIZER_100_GAUGE_SHIFTBY, onFueltotalizer100Change);
DcsBios::IntegerBuffer fueltotalizer1kBuffer(FUELTOTALIZER_1K_GAUGE_ADDRESS, FUELTOTALIZER_1K_GAUGE_MASK,  FUELTOTALIZER_1K_GAUGE_SHIFTBY, onFueltotalizer1kChange);
DcsBios::IntegerBuffer fueltotalizer10kBuffer(FUELTOTALIZER_10K_GAUGE_ADDRESS, FUELTOTALIZER_10K_GAUGE_MASK, FUELTOTALIZER_10K_GAUGE_SHIFTBY, onFueltotalizer10kChange);
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);

// Arduino standard functions
void setup() {
    display_totalizer.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    stepper_init();
    displayFuel();
    steppers_sweep();
    DcsBios::setup();
}

void loop() {
    DcsBios::loop();
}