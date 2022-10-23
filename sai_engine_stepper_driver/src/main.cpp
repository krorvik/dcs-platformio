#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>
#include <i2c.h>
#include <stepper.h>
#include <f16c_data.h>
#include <helpers.h>


ViperStepper       oil_stepper( 720,    0,  600,  200, 10000, 10);
ViperStepper       noz_stepper( 720,    0,  475,  200, 10000, 10);
ViperStepper       rpm_stepper( 720,    0,  600,  200, 10000, 10);
ViperStepper      ftit_stepper( 720,    0,  625,  200, 10000, 10);
ViperStepper  sai_bank_stepper( 200, -100,  100,  200,  1500, 1);
ViperStepper sai_pitch_stepper( 340,  170,  -170, 1000,  2000, 5);

// DCS Bios callback setup, this is where the instruments meet the callbacks - and the steppers are assigned
void onEngineOilPressureChange(unsigned int newValue)    { oil_stepper.moveToBounded(newValue); } 
void onEngineNozzlePositionChange(unsigned int newValue) { noz_stepper.moveToBounded(newValue); }
void onEngineTachometerChange(unsigned int newValue)     { rpm_stepper.moveToBounded(newValue); }
void onEngineFtitChange(unsigned int newValue)           { ftit_stepper.moveToBounded(newValue); }
void onSaiBankChange(unsigned int newValue)              { sai_bank_stepper.moveToContinuous(newValue); }
void onSaiPitchChange(unsigned int newValue)             { sai_pitch_stepper.moveToBounded(newValue); }
void onFuelflowcounter100Change(unsigned int newValue)   { fuelflow_chars[2] = '0' + translate_digit(newValue); }
void onFuelflowcounter1kChange(unsigned int newValue)    { fuelflow_chars[1] = '0' + translate_digit(newValue); }
void onFuelflowcounter10kChange(unsigned int newValue)   { fuelflow_chars[0] = '0' + translate_digit(newValue); }

// SAI and Engine instruments for this one
DcsBios::IntegerBuffer saiBankBuffer             (SAI_BANK_GAUGE_ADDRESS,               SAI_BANK_GAUGE_MASK,               SAI_BANK_GAUGE_SHIFTBY,               onSaiBankChange);
DcsBios::IntegerBuffer saiPitchBuffer            (SAI_PITCH_GAUGE_ADDRESS,              SAI_PITCH_GAUGE_MASK,              SAI_BANK_GAUGE_SHIFTBY,               onSaiPitchChange);
DcsBios::IntegerBuffer engineOilPressureBuffer   (ENGINE_OIL_PRESSURE_GAUGE_ADDRESS,    ENGINE_OIL_PRESSURE_GAUGE_MASK,    ENGINE_OIL_PRESSURE_GAUGE_SHIFTBY,    onEngineOilPressureChange);
DcsBios::IntegerBuffer engineNozzlePositionBuffer(ENGINE_NOZZLE_POSITION_GAUGE_ADDRESS, ENGINE_NOZZLE_POSITION_GAUGE_MASK, ENGINE_NOZZLE_POSITION_GAUGE_SHIFTBY, onEngineNozzlePositionChange);
DcsBios::IntegerBuffer engineTachometerBuffer    (ENGINE_TACHOMETER_GAUGE_ADDRESS,      ENGINE_TACHOMETER_GAUGE_MASK,      ENGINE_TACHOMETER_GAUGE_SHIFTBY,      onEngineTachometerChange);
DcsBios::IntegerBuffer engineFtitBuffer          (ENGINE_FTIT_GAUGE_ADDRESS,            ENGINE_FTIT_GAUGE_MASK,            ENGINE_FTIT_GAUGE_SHIFTBY,            onEngineFtitChange);
DcsBios::IntegerBuffer fuelflowcounter100Buffer  (FUELFLOWCOUNTER_100_GAUGE_ADDRESS,    FUELFLOWCOUNTER_100_GAUGE_MASK,    FUELFLOWCOUNTER_100_GAUGE_SHIFTBY,    onFuelflowcounter100Change);
DcsBios::IntegerBuffer fuelflowcounter1kBuffer   (FUELFLOWCOUNTER_1K_GAUGE_ADDRESS,     FUELFLOWCOUNTER_1K_GAUGE_MASK,     FUELFLOWCOUNTER_1K_GAUGE_SHIFTBY,     onFuelflowcounter1kChange);
DcsBios::IntegerBuffer fuelflowcounter10kBuffer  (FUELFLOWCOUNTER_10K_GAUGE_ADDRESS,    FUELFLOWCOUNTER_10K_GAUGE_MASK,    FUELFLOWCOUNTER_10K_GAUGE_SHIFTBY,    onFuelflowcounter10kChange);

void setup() {
  // sai_bank_stepper.adjustFactor = 2;
  stepper_init();
  display_init();
  DcsBios::setup();
}

void loop() {
  stepper_loop();
  if (initAllowed()) {
    write_display_mux(2, String(getStepperID() ), 55, 0, 4) ;
  } else {
    write_display(fuelflow_chars, 0, 0, 4);
  }
  DcsBios::loop();
}