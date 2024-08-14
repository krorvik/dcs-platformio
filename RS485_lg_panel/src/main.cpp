#define DCSBIOS_RS485_SLAVE 50
#define TXENABLE_PIN 2
#include <DcsBios.h>
#include "f16c_data.h"
#include "Button2.h"

#define sensorcount 2
#define LG_HANDLE_UP 0
#define LG_HANDLE_DOWN 1

const int lg_led_pins[3] = { 3, 6, 5 };
const int lg_warn_pin = 4;

Button2 lg_up_button;
Button2 lg_dn_button;

void initLeds() {
  for(int ix = 3; ix <= 6; ix++) {
    pinMode(ix, OUTPUT);    
  }

  digitalWrite(4, HIGH);
  for (int iz = 0; iz < 3; iz++) {
    analogWrite(lg_led_pins[iz], 16);
  }

  delay(2000);
  digitalWrite(4, LOW);
  for(int iz = 0; iz < 3; iz++) {
    analogWrite(lg_led_pins[iz], 0);    
  }
}

void lg_up(Button2& button) {
  sendDcsBiosMessage("GEAR_HANDLE", "1");
};
void lg_dn(Button2& button) {
  sendDcsBiosMessage("GEAR_HANDLE", "0");
};

void onLightGearLChange(unsigned int newValue) { analogWrite(lg_led_pins[0], 16 * newValue); }
void onLightGearNChange(unsigned int newValue) { analogWrite(lg_led_pins[1], 16 * newValue); }
void onLightGearRChange(unsigned int newValue) { analogWrite(lg_led_pins[2], 16 * newValue); }

DcsBios::Switch2Pos hookSw("HOOK_SW", 7);
DcsBios::Switch2Pos emergStroreJett("EMERG_STRORE_JETT", 8);
DcsBios::Switch3Pos antiSkidSw("ANTI_SKID_SW", 9, 10);
DcsBios::Switch2Pos brakeChanSw("BRAKE_CHAN_SW", 11);
DcsBios::Switch2Pos gndJettEnableSw("GND_JETT_ENABLE_SW", 12);
DcsBios::Switch2Pos storesConfigSw("STORES_CONFIG_SW", 13);
DcsBios::Switch2Pos hornSilenceBtn("HORN_SILENCE_BTN", A0);
DcsBios::Switch3Pos landTaxiLightSw("LAND_TAXI_LIGHT_SW", A1, A2);
DcsBios::Switch2Pos dnLockBtn("DN_LOCK_BTN", A3);

// LEDs
DcsBios::IntegerBuffer lightGearLBuffer(LIGHT_GEAR_L_LED_ADDRESS, LIGHT_GEAR_L_LED_MASK, LIGHT_GEAR_L_LED_SHIFTBY, onLightGearLChange);
DcsBios::IntegerBuffer lightGearNBuffer(LIGHT_GEAR_N_LED_ADDRESS, LIGHT_GEAR_N_LED_MASK, LIGHT_GEAR_N_LED_SHIFTBY, onLightGearNChange);
DcsBios::IntegerBuffer lightGearRBuffer(LIGHT_GEAR_R_LED_ADDRESS, LIGHT_GEAR_R_LED_MASK, LIGHT_GEAR_R_LED_SHIFTBY, onLightGearRChange);
DcsBios::LED lightGearWarn(LIGHT_GEAR_WARN_LED_ADDRESS, LIGHT_GEAR_WARN_LED_MASK, lg_warn_pin);

void setup() {
  // put your setup code here, to run once:
  initLeds();
  lg_up_button.begin(A4);
  lg_dn_button.begin(A5);  
  lg_dn_button.setPressedHandler(lg_dn);
  lg_up_button.setPressedHandler(lg_up);
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
  lg_dn_button.loop();
  lg_up_button.loop();
}
