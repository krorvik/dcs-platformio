#define DCSBIOS_RS485_SLAVE 65
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include <Adafruit_PWMServoDriver.h>
#include "f16c_data.h"

#define PITCH_SERVO 0
#define ROLL_SERVO 1
#define DISC_SWITCH_PIN 12
#define YAW_TRIM_PIN A0

Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver();

const int pitch_min = 125;
const int roll_min = 125;

const int pitch_mid = 350;
const int roll_mid = 350;

const int pitch_max = 575;
const int roll_max = 575;

// callback functions
void onPitchtrimindChange(unsigned int newValue) { servos.setPWM(PITCH_SERVO, 0, map(newValue, 0, 65535, pitch_min, pitch_max)); }
void onRolltrimindChange(unsigned int newValue) { servos.setPWM(ROLL_SERVO, 0, map(newValue, 0, 65535, roll_min, roll_max)); }

// DCS Bios callback defs
DcsBios::IntegerBuffer rolltrimindBuffer(ROLLTRIMIND_GAUGE_ADDRESS, ROLLTRIMIND_GAUGE_MASK, ROLLTRIMIND_GAUGE_SHIFTBY, onRolltrimindChange);
DcsBios::IntegerBuffer pitchtrimindBuffer(PITCHTRIMIND_GAUGE_ADDRESS, PITCHTRIMIND_GAUGE_MASK, PITCHTRIMIND_GAUGE_SHIFTBY, onPitchtrimindChange);
DcsBios::Switch2Pos trimApDiscSw("TRIM_AP_DISC_SW", DISC_SWITCH_PIN);
DcsBios::Potentiometer yawTrim("YAW_TRIM", YAW_TRIM_PIN);

void initSweep() {
  servos.setPWM(PITCH_SERVO, 0, pitch_min);
  servos.setPWM(ROLL_SERVO, 0, roll_min);
  delay(2000);
  servos.setPWM(PITCH_SERVO, 0, pitch_max);
  servos.setPWM(ROLL_SERVO, 0, roll_max);
  delay(2000);
  servos.setPWM(PITCH_SERVO, 0, pitch_mid);
  servos.setPWM(ROLL_SERVO, 0, roll_mid);

}

void setup() {
  pinMode(DISC_SWITCH_PIN, INPUT_PULLUP);
  pinMode(YAW_TRIM_PIN, INPUT);
  // put your setup code here, to run once:
  servos.begin();
  servos.setPWMFreq(60);
  initSweep();
  
  DcsBios::setup();

}

void loop() {
  // put your main code here, to run repeatedly:
  DcsBios::loop();
}
