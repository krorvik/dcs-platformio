#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>
#include <display.h>
#include <stepper.h>
#include <dcs_data.h>
#include <helpers.h>


//ViperStepper       oil_stepper( 720,    0,  600,  200, 10000, 10);

// DCS Bios callback setup, this is where the instruments meet the callbacks - and the steppers are assigned
// void onEngineOilPressureChange(unsigned int newValue)    { oil_stepper.moveToBounded(newValue); } 
void onMainPwrSwChange(unsigned int newValue)            { disable_init(); }

// SAI and Engine instruments for this one
//& DcsBios::IntegerBuffer engineOilPressureBuffer   (ENGINE_OIL_PRESSURE_GAUGE_ADDRESS,    ENGINE_OIL_PRESSURE_GAUGE_MASK,    ENGINE_OIL_PRESSURE_GAUGE_SHIFTBY,    onEngineOilPressureChange);
DcsBios::IntegerBuffer mainPwrSwBuffer           (MAIN_PWR_SW_SELECTOR_ADDRESS,         MAIN_PWR_SW_SELECTOR_MASK,         MAIN_PWR_SW_SELECTOR_SHIFTBY,         onMainPwrSwChange);

void setup() {
  // sai_bank_stepper.adjustFactor = 2;
  stepper_init();
  display_init();
  DcsBios::setup();
}

void loop() {
  stepper_loop();
  if (initAllowed()) {
    write_display(String(getStepperID()) + " " + String(lastpos));
  } else {
    // Update displays
  }
  DcsBios::loop();
}