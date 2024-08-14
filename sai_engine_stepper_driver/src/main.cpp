#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>
#include <Adafruit_SSD1306.h>
#include <stepper.h>
#include <f16c_data.h>
#include <helpers.h>

char fuelflow_chars[6] = {'0', '0', '0', '0', '0', '\0'};

// Note: VID28 really does not like higher supply voltages to the easydriver. 
// Landed at 5V max for good performance. 
ViperStepper       oil_stepper( 720,    0,  590,  600, 100000);
ViperStepper       noz_stepper( 720,    0,  500,  600, 100000);
ViperStepper       rpm_stepper( 720,    0,  620,  600, 100000);
ViperStepper      ftit_stepper( 720,    0,  670,  600, 100000);

void display_fuelflow(unsigned int newValue) {
  write_display(fuelflow_chars, 24, 6, 3);
}

// DCS Bios callback setup, this is where the instruments meet the callbacks - and the steppers are assigned
void onEngineOilPressureChange(unsigned int newValue)    { oil_stepper.moveTo(newValue); } 
void onEngineNozzlePositionChange(unsigned int newValue) { noz_stepper.moveTo(newValue); }
void onEngineTachometerChange(unsigned int newValue)     { rpm_stepper.moveTo(newValue); }
void onEngineFtitChange(unsigned int newValue)           { ftit_stepper.moveTo(newValue); }
void onFuelflowcounter100Change(unsigned int newValue)   { fuelflow_chars[2] = '0' + translate_digit(newValue); }
void onFuelflowcounter1kChange(unsigned int newValue)    { fuelflow_chars[1] = '0' + translate_digit(newValue); }
void onFuelflowcounter10kChange(unsigned int newValue)   { fuelflow_chars[0] = '0' + translate_digit(newValue); }

DcsBios::IntegerBuffer engineOilPressureBuffer   (ENGINE_OIL_PRESSURE_GAUGE_ADDRESS,    ENGINE_OIL_PRESSURE_GAUGE_MASK,    ENGINE_OIL_PRESSURE_GAUGE_SHIFTBY,    onEngineOilPressureChange);
DcsBios::IntegerBuffer engineNozzlePositionBuffer(ENGINE_NOZZLE_POSITION_GAUGE_ADDRESS, ENGINE_NOZZLE_POSITION_GAUGE_MASK, ENGINE_NOZZLE_POSITION_GAUGE_SHIFTBY, onEngineNozzlePositionChange);
DcsBios::IntegerBuffer engineTachometerBuffer    (ENGINE_TACHOMETER_GAUGE_ADDRESS,      ENGINE_TACHOMETER_GAUGE_MASK,      ENGINE_TACHOMETER_GAUGE_SHIFTBY,      onEngineTachometerChange);
DcsBios::IntegerBuffer engineFtitBuffer          (ENGINE_FTIT_GAUGE_ADDRESS,            ENGINE_FTIT_GAUGE_MASK,            ENGINE_FTIT_GAUGE_SHIFTBY,            onEngineFtitChange);
DcsBios::IntegerBuffer fuelflowcounter100Buffer  (FUELFLOWCOUNTER_100_GAUGE_ADDRESS,    FUELFLOWCOUNTER_100_GAUGE_MASK,    FUELFLOWCOUNTER_100_GAUGE_SHIFTBY,    onFuelflowcounter100Change);
DcsBios::IntegerBuffer fuelflowcounter1kBuffer   (FUELFLOWCOUNTER_1K_GAUGE_ADDRESS,     FUELFLOWCOUNTER_1K_GAUGE_MASK,     FUELFLOWCOUNTER_1K_GAUGE_SHIFTBY,     onFuelflowcounter1kChange);
DcsBios::IntegerBuffer fuelflowcounter10kBuffer  (FUELFLOWCOUNTER_10K_GAUGE_ADDRESS,    FUELFLOWCOUNTER_10K_GAUGE_MASK,    FUELFLOWCOUNTER_10K_GAUGE_SHIFTBY,    onFuelflowcounter10kChange);
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, display_fuelflow);

void setup() {
  stepper_init();
  display_init();
  display_fuelflow(0);
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}