#define DCSBIOS_DEFAULT_SERIAL
// #define DCSBIOS_DISABLE_SERVO
// #include <DcsBios.h>
// #include <f16c_data.h>
// #include <common_data.h>
#include <viperstepper.h>

// max vakues:
// oil: 590
// noz: 500
// rpm: 590
// ftit: 500
//                 res min max   spd   accel
ViperStepper stp_a(720, 0, 630, 100, 10000);
ViperStepper stp_b(720, 0, 630, 100, 10000);
ViperStepper stp_c(720, 0, 630, 100, 10000);
ViperStepper stp_d(720, 0, 630, 100, 10000);

/*
void onEngineOilPressureChange(unsigned int newValue)    { stp_a.moveTo(newValue); } 
void onEngineNozzlePositionChange(unsigned int newValue) { stp_b.moveTo(newValue); }
void onEngineRPMChange(unsigned int newValue)    { stp_c.moveTo(newValue); } 
void onEngineFTITChange(unsigned int newValue) { stp_d.moveTo(newValue); }

DcsBios::IntegerBuffer engineOilPressureBuffer   (ENGINE_OIL_PRESSURE_GAUGE_ADDRESS,    ENGINE_OIL_PRESSURE_GAUGE_MASK,    ENGINE_OIL_PRESSURE_GAUGE_SHIFTBY,    onEngineOilPressureChange);
DcsBios::IntegerBuffer engineNozzlePositionBuffer(ENGINE_NOZZLE_POSITION_GAUGE_ADDRESS, ENGINE_NOZZLE_POSITION_GAUGE_MASK, ENGINE_NOZZLE_POSITION_GAUGE_SHIFTBY, onEngineNozzlePositionChange);
DcsBios::IntegerBuffer engineRPMBuffer   (ENGINE_TACHOMETER_GAUGE_ADDRESS,    ENGINE_TACHOMETER_GAUGE_MASK,    ENGINE_TACHOMETER_GAUGE_SHIFTBY,  onEngineRPMChange);
DcsBios::IntegerBuffer engineFTITBuffer(ENGINE_FTIT_GAUGE_ADDRESS, ENGINE_FTIT_GAUGE_MASK, ENGINE_FTIT_GAUGE_SHIFTBY, onEngineFTITChange);
*/

void stepper_sweep() {
  for(unsigned int i = 0; i < 8; i++) {
    // Raw steps
    // stp_a.move(50000);
  }
}

void setup(  ) {
  stepper_init();
  stepper_sweep();
  // DcsBios::setup();
}

void loop() {
  // DcsBios::loop();
}


