#define DCSBIOS_RS485_SLAVE 48
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"
#include "FastAccelStepper.h"
#include "Adafruit_SSD1306.h"

// Stepper defs, speed and accel are same (VID2x)
#define SPEED 5000 // About a second for a sweep ;)
#define ACCELERATION 65535
// Ardiuno pins - always the same pairs
#define STEP_PIN_A 10
#define DIR_PIN_A 12
#define STEP_PIN_B 9
#define DIR_PIN_B 11
// Range of stepper in steps, 360 and range of gauge
#define STEPS_MAX 5760
#define STEPS_MAX_A  5040 
#define STEPS_MAX_B  5040
// Display defs
#define DISPLAY_WIDTH 128 // OLED display width, in pixels
#define DISPLAY_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1 
#define DISPLAY_ADDRESS 0x3C

// totalizer display digits
char totalizer_digits[6] = { '0', '0', '0', '0', '0', '\0' };

// Steppers
FastAccelStepperEngine stepper_engine = FastAccelStepperEngine();
FastAccelStepper *stepper_a = NULL;
FastAccelStepper *stepper_b = NULL;
// Display, standard I2C
Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT);


// Stepper functions
void stepper_init() {
  stepper_engine.init();
  stepper_a = stepper_engine.stepperConnectToPin(STEP_PIN_A);
  stepper_a->setDirectionPin(DIR_PIN_A, false); // false switches direction
  stepper_a->setSpeedInHz(SPEED);
  stepper_a->setAcceleration(ACCELERATION);
  stepper_a->setCurrentPosition(0);
  stepper_b = stepper_engine.stepperConnectToPin(STEP_PIN_B);
  stepper_b->setDirectionPin(DIR_PIN_B);
  stepper_b->setSpeedInHz(SPEED);
  stepper_b->setAcceleration(ACCELERATION);
  stepper_b->setCurrentPosition(0);
}

void steppers_sweep() {
    stepper_a->moveTo(-5760);
    stepper_b->moveTo(-5760);
    while(stepper_a->isRunning()) {}
    while(stepper_b->isRunning()) {}
    stepper_a->setCurrentPosition(0);
    stepper_b->setCurrentPosition(0);
    stepper_a->moveTo(STEPS_MAX_A);
    stepper_b->moveTo(STEPS_MAX_B);
    while(stepper_a->isRunning()) {}
    while(stepper_b->isRunning()) {}
    delay(1000);
    stepper_a->moveTo(0);
    stepper_b->moveTo(0);
    while(stepper_b->isRunning()) {}
    while(stepper_a->isRunning()) {}
}

// Fuel display function

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

char* fuel_blanked() {
  char *tmp_digits = totalizer_digits;
  if (totalizer_digits[0] == '0') {
    tmp_digits[0] = ' ';
    if (totalizer_digits[1] == '0' ) { tmp_digits[1] = ' ';}
  }
  return tmp_digits;
}

void display_totalizer(unsigned int newValue) {
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20,5);
  display.println(fuel_blanked());
  display.display();
}

// callback functions, move steppers ;)
void onSysaPressureChange(unsigned int newValue)     { stepper_a->moveTo(map(newValue, 0, 65535, 0, STEPS_MAX_A)); }
void onSysbPressureChange(unsigned int newValue)     { stepper_b->moveTo(map(newValue, 0, 65535, 0, STEPS_MAX_B)); }
void onFueltotalizer100Change(unsigned int newValue) { totalizer_digits[2] = '0' + translate_digit(newValue); }
void onFueltotalizer1kChange(unsigned int newValue)  { totalizer_digits[1] = '0' + translate_digit(newValue); }
void onFueltotalizer10kChange(unsigned int newValue) { totalizer_digits[0] = '0' + translate_digit(newValue); }
// DCS Bios callback setup, this is where the instruments meet the callbacks - and the steppers are assigned
DcsBios::IntegerBuffer sysaPressureBuffer(SYSA_PRESSURE_GAUGE_ADDRESS , SYSA_PRESSURE_GAUGE_MASK , SYSA_PRESSURE_GAUGE_SHIFTBY , onSysaPressureChange);
DcsBios::IntegerBuffer sysbPressureBuffer(SYSB_PRESSURE_GAUGE_ADDRESS , SYSB_PRESSURE_GAUGE_MASK , SYSB_PRESSURE_GAUGE_SHIFTBY , onSysbPressureChange);
DcsBios::IntegerBuffer fueltotalizer100Buffer(FUELTOTALIZER_100_GAUGE_ADDRESS , FUELTOTALIZER_100_GAUGE_MASK, FUELTOTALIZER_100_GAUGE_SHIFTBY , onFueltotalizer100Change);
DcsBios::IntegerBuffer fueltotalizer1kBuffer(FUELTOTALIZER_1K_GAUGE_ADDRESS , FUELTOTALIZER_1K_GAUGE_MASK , FUELTOTALIZER_1K_GAUGE_SHIFTBY, onFueltotalizer1kChange);
DcsBios::IntegerBuffer fueltotalizer10kBuffer(FUELTOTALIZER_10K_GAUGE_ADDRESS , FUELTOTALIZER_10K_GAUGE_MASK , FUELTOTALIZER_10K_GAUGE_SHIFTBY , onFueltotalizer10kChange);
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, display_totalizer);

// Arduino standard functions
void setup() {
    display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS);
    display_totalizer(0);
    stepper_init();
    steppers_sweep();
    DcsBios::setup();
}

void loop() {
    DcsBios::loop();
}