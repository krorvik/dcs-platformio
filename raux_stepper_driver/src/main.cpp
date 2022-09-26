#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>
#include <i2c.h>
#include <stepper.h>
#include <dcs_data.h>
#include <helpers.h>

ViperStepper        compass_stepper ( 720,   0,  720, 800,  5000, 5);
ViperStepper        fuel_fr_stepper ( 720,   0,  720, 800,  5000, 5);
ViperStepper        fuel_al_stepper ( 720,   0,  720, 800,  5000, 5);
ViperStepper        hyda_stepper    ( 720,   0,  720, 800,  5000, 5);
ViperStepper        hydb_stepper    ( 720,   0,  720, 800,  5000, 5);
ViperStepper        epu_stepper     ( 720,   0,  720, 800,  5000, 5);
ViperStepper        oxy_stepper     ( 720,   0,  720, 800,  5000, 5);

// DCS callbacks
void onHdgDegChange(unsigned int newValue)           { compass_stepper.moveToContinuous(newValue); }
void onFuelAlChange(unsigned int newValue)           { fuel_al_stepper.moveToBounded(newValue); }
void onFuelFrChange(unsigned int newValue)           { fuel_fr_stepper.moveToBounded(newValue); }
void onSysaPressureChange(unsigned int newValue)     { hyda_stepper.moveToBounded(newValue); }
void onSysbPressureChange(unsigned int newValue)     { hydb_stepper.moveToBounded(newValue); }
void onCockpitAlititudeChange(unsigned int newValue) { oxy_stepper.moveToBounded(newValue); }
void onHydrazinVolumeChange(unsigned int newValue)   { epu_stepper.moveToBounded(newValue); }
void onFueltotalizer100Change(unsigned int newValue) { totalizer_digits[2] = '0' + translateDigit(newValue); }
void onFueltotalizer1kChange(unsigned int newValue)  { totalizer_digits[1] = '0' + translateDigit(newValue); }
void onFueltotalizer10kChange(unsigned int newValue) { totalizer_digits[0] = '0' + translateDigit(newValue); }

DcsBios::IntegerBuffer hdgDegBuffer(0x0436, 0x01ff, 0, onHdgDegChange);
DcsBios::IntegerBuffer fuelAlBuffer(FUEL_AL_GAUGE_ADDRESS , FUEL_AL_GAUGE_MASK , FUEL_AL_GAUGE_SHIFTBY , onFuelAlChange);
DcsBios::IntegerBuffer fuelFrBuffer(FUEL_FR_GAUGE_ADDRESS , FUEL_FR_GAUGE_MASK , FUEL_FR_GAUGE_SHIFTBY , onFuelFrChange);
DcsBios::IntegerBuffer sysbPressureBuffer(SYSA_PRESSURE_GAUGE_ADDRESS , SYSA_PRESSURE_GAUGE_MASK , SYSA_PRESSURE_GAUGE_SHIFTBY , onSysbPressureChange);
DcsBios::IntegerBuffer sysaPressureBuffer(SYSB_PRESSURE_GAUGE_ADDRESS , SYSB_PRESSURE_GAUGE_MASK , SYSB_PRESSURE_GAUGE_SHIFTBY , onSysaPressureChange);
DcsBios::IntegerBuffer cockpitAlititudeBuffer(COCKPIT_ALITITUDE_GAUGE_ADDRESS , COCKPIT_ALITITUDE_GAUGE_MASK, COCKPIT_ALITITUDE_GAUGE_SHIFTBY, onCockpitAlititudeChange);
DcsBios::IntegerBuffer hydrazinVolumeBuffer(HYDRAZIN_VOLUME_GAUGE_ADDRESS , HYDRAZIN_VOLUME_GAUGE_MASK, HYDRAZIN_VOLUME_GAUGE_SHIFTBY , onHydrazinVolumeChange);
DcsBios::IntegerBuffer fueltotalizer100Buffer(FUELTOTALIZER_100_GAUGE_ADDRESS , FUELTOTALIZER_100_GAUGE_MASK, FUELTOTALIZER_100_GAUGE_SHIFTBY , onFueltotalizer100Change);
DcsBios::IntegerBuffer fueltotalizer1kBuffer(FUELTOTALIZER_1K_GAUGE_ADDRESS , FUELTOTALIZER_1K_GAUGE_MASK , FUELTOTALIZER_1K_GAUGE_SHIFTBY, onFueltotalizer1kChange);
DcsBios::IntegerBuffer fueltotalizer10kBuffer(FUELTOTALIZER_10K_GAUGE_ADDRESS , FUELTOTALIZER_10K_GAUGE_MASK , FUELTOTALIZER_10K_GAUGE_SHIFTBY , onFueltotalizer10kChange);

void setup() {
  stepper_init();
  display_init();
  DcsBios::setup();
}

void loop() {
  stepper_loop();
  if (initAllowed()) {
    write_display(String(getStepperID() ), 55, 0, 4);
  } else {
    write_display(fuel_blanked(), 0, 0, 4);
  }
  DcsBios::loop();
}