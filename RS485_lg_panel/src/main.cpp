#define DCSBIOS_RS485_SLAVE 50
#define TXENABLE_PIN 2
#include <DcsBios.h>
#include "f16c_data.h"

#define sensorcount 2
#define LG_HANDLE_UP 0
#define LG_HANDLE_DOWN 1

const int lg_led_pins[3] = { 3, 4, 5 };
const int lg_warn_pin = 6;
const int sensor_pins[sensorcount] = { A6, A7 };

// Sensor values
int sensor_values[sensorcount]= { 0, 0 };
int handle_position = -1; // Start with invalid
int command_position = -1;

void initSensors() {
  for (int ix = 0; ix < sensorcount; ix++) {
    pinMode(sensor_pins[ix], INPUT);
  }
}

void initLeds() {
  for(int ix = 3; ix <= 6; ix++) {
    pinMode(ix, OUTPUT);
    digitalWrite(ix, HIGH);
  }
  delay(2000);
  for(int ix = 3; ix <= 6; ix++) {
    digitalWrite(ix, LOW);
  }
}

void readSensors() {
  for (int ix = 0; ix < sensorcount; ix++) {
    sensor_values[ix] = analogRead(sensor_pins[ix]);
  }  
}

void set_handle_position() {
  // Handle UP
  if (sensor_values[0] > 512 and sensor_values[1] < 512) {
    handle_position = LG_HANDLE_UP;    
  // Handle DOWN
  } else if (sensor_values[0] < 512 and sensor_values[1] > 512) {
    handle_position = LG_HANDLE_DOWN;
  } // Else do not act - handle is traveling
}

void emit_dcs_command() {
  if(command_position != handle_position) {
    if (handle_position == LG_HANDLE_DOWN) { sendDcsBiosMessage("GEAR_HANDLE", "0");  }
    if (handle_position == LG_HANDLE_UP) { sendDcsBiosMessage("GEAR_HANDLE",  "1"); }
    command_position = handle_position;
  }  
}


DcsBios::Switch3Pos antiSkidSw("ANTI_SKID_SW", 9, 10);
DcsBios::Switch2Pos brakeChanSw("BRAKE_CHAN_SW", 11);
DcsBios::Switch2Pos dnLockBtn("DN_LOCK_BTN", 17);
DcsBios::Switch2Pos hookSw("HOOK_SW", 7);
DcsBios::Switch2Pos hornSilenceBtn("HORN_SILENCE_BTN", 14);
DcsBios::Switch3Pos landTaxiLightSw("LAND_TAXI_LIGHT_SW", 15, 16);
DcsBios::Switch2Pos storesConfigSw("STORES_CONFIG_SW", 13);
DcsBios::Switch2Pos gndJettEnableSw("GND_JETT_ENABLE_SW", 12);
DcsBios::Switch2Pos emergStroreJett("EMERG_STRORE_JETT", 8);

// LEDs
DcsBios::LED lightGearL(LIGHT_GEAR_L_LED_ADDRESS, LIGHT_GEAR_L_LED_MASK, lg_led_pins[0]);
DcsBios::LED lightGearN(LIGHT_GEAR_N_LED_ADDRESS, LIGHT_GEAR_N_LED_MASK, lg_led_pins[1]);
DcsBios::LED lightGearR(LIGHT_GEAR_R_LED_ADDRESS, LIGHT_GEAR_R_LED_MASK, lg_led_pins[2]);
DcsBios::LED lightGearWarn(LIGHT_GEAR_WARN_LED_ADDRESS, LIGHT_GEAR_WARN_LED_MASK, lg_warn_pin);

void setup() {
  // put your setup code here, to run once:
  initLeds();
  initSensors();
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
  readSensors();
  set_handle_position();
  emit_dcs_command();  
}
