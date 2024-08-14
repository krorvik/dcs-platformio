#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>
#include <stepper.h>
#include <f16c_data.h>
#include <common_data.h>
#include "i2c.h"

ViperStepper        hyda_stepper    ( 720,   0,  630, 600,  100000);
ViperStepper        hydb_stepper    ( 720,   0,  630, 600,  100000);
ViperStepper        epu_stepper     ( 720,   0,  550, 600,  100000);
ViperStepper        oxy_stepper     ( 720,   0,  600, 600,  100000);
ViperStepper        fuel_fr_stepper ( 720,   0,  620, 600,  100000); // Gauge in sim seems to be calibrated for full 360 degrees, reduce maxval correspondingly
ViperStepper        fuel_al_stepper ( 720,   0,  620, 600,  100000); // Same same sim

char totalizer_digits[6] = { '0', '0', '0', '0', '0', '\0' };

char* fuel_blanked() {
  char *tmp_digits = totalizer_digits;
  if (totalizer_digits[0] == '0') {
    tmp_digits[0] = ' ';
    if (totalizer_digits[1] == '0' ) { tmp_digits[1] = ' ';}
  }
  return tmp_digits;
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

void display_totalizer(unsigned int newValue) {
  write_display(fuel_blanked(), 20, 5, 3);
}

// DCS callbacks
void onSysaPressureChange(unsigned int newValue)     { hyda_stepper.moveTo(newValue); }
void onSysbPressureChange(unsigned int newValue)     { hydb_stepper.moveTo(newValue); }
void onCockpitAlititudeChange(unsigned int newValue) { oxy_stepper.moveTo(newValue); }
void onHydrazinVolumeChange(unsigned int newValue)   { epu_stepper.moveTo(newValue); }
void onFuelFrChange(unsigned int newValue)           { fuel_fr_stepper.moveTo(newValue); }
void onFuelAlChange(unsigned int newValue)           { fuel_al_stepper.moveTo(newValue); }

void onFueltotalizer100Change(unsigned int newValue) { totalizer_digits[2] = '0' + translate_digit(newValue); }
void onFueltotalizer1kChange(unsigned int newValue)  { totalizer_digits[1] = '0' + translate_digit(newValue); }
void onFueltotalizer10kChange(unsigned int newValue) { totalizer_digits[0] = '0' + translate_digit(newValue); }

DcsBios::IntegerBuffer sysaPressureBuffer(SYSA_PRESSURE_GAUGE_ADDRESS , SYSA_PRESSURE_GAUGE_MASK , SYSA_PRESSURE_GAUGE_SHIFTBY , onSysaPressureChange);
DcsBios::IntegerBuffer sysbPressureBuffer(SYSB_PRESSURE_GAUGE_ADDRESS , SYSB_PRESSURE_GAUGE_MASK , SYSB_PRESSURE_GAUGE_SHIFTBY , onSysbPressureChange);
DcsBios::IntegerBuffer cockpitAlititudeBuffer(COCKPIT_ALITITUDE_GAUGE_ADDRESS , COCKPIT_ALITITUDE_GAUGE_MASK, COCKPIT_ALITITUDE_GAUGE_SHIFTBY, onCockpitAlititudeChange);
DcsBios::IntegerBuffer hydrazinVolumeBuffer(HYDRAZIN_VOLUME_GAUGE_ADDRESS , HYDRAZIN_VOLUME_GAUGE_MASK, HYDRAZIN_VOLUME_GAUGE_SHIFTBY , onHydrazinVolumeChange);
DcsBios::IntegerBuffer fuelFrBuffer(FUEL_FR_GAUGE_ADDRESS , FUEL_FR_GAUGE_MASK , FUEL_FR_GAUGE_SHIFTBY , onFuelFrChange);
DcsBios::IntegerBuffer fuelAlBuffer(FUEL_AL_GAUGE_ADDRESS , FUEL_AL_GAUGE_MASK , FUEL_AL_GAUGE_SHIFTBY , onFuelAlChange);
DcsBios::IntegerBuffer fueltotalizer100Buffer(FUELTOTALIZER_100_GAUGE_ADDRESS , FUELTOTALIZER_100_GAUGE_MASK, FUELTOTALIZER_100_GAUGE_SHIFTBY , onFueltotalizer100Change);
DcsBios::IntegerBuffer fueltotalizer1kBuffer(FUELTOTALIZER_1K_GAUGE_ADDRESS , FUELTOTALIZER_1K_GAUGE_MASK , FUELTOTALIZER_1K_GAUGE_SHIFTBY, onFueltotalizer1kChange);
DcsBios::IntegerBuffer fueltotalizer10kBuffer(FUELTOTALIZER_10K_GAUGE_ADDRESS , FUELTOTALIZER_10K_GAUGE_MASK , FUELTOTALIZER_10K_GAUGE_SHIFTBY , onFueltotalizer10kChange);
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, display_totalizer);

void debug() {
  fuel_al_stepper.moveTo(26279);
  fuel_fr_stepper.moveTo(26279);
  while (anyRunning()) {}
  delay(3000);
  fuel_al_stepper.moveTo(0);
  fuel_fr_stepper.moveTo(0);
}

void setup() {
  stepper_init();
  display_init();
  display_totalizer(0);
  while(anyRunning()) {}
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}