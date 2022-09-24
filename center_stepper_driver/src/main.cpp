#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>
#include <display.h>
#include <stepper.h>
#include <dcs_data.h>
#include <helpers.h>

ViperStepper        airspeed_stepper (720, 0, 720, 200, 10000, 10);
ViperStepper        altimeter_stepper(720, 0, 720, 200, 10000, 10);
ViperStepper        aoa_stepper      (720, 0, 720, 200, 10000, 10);
ViperStepper        adi_bank_stepper (200, -800,  800,  200,  2000,  1);
ViperStepper        adi_pitch_stepper(800,-5500, 5500, 2000,  2000, 10);
ViperStepper        vvi_stepper      (720, 0, 720, 200, 10000, 10);

// DCS Bios callback setup, this is where the instruments meet the callbacks - and the steppers are assigned
void onMainPwrSwChange(unsigned int newValue)            { disable_init(); }

void onAdiTurnrateChange(unsigned int newValue) {}
void onAdiAuxFlagChange(unsigned int newValue) {}
void onAdiLocFlagChange(unsigned int newValue) {}
void onAdiOffFlagChange(unsigned int newValue) {}
void onAdiGsFlagChange(unsigned int newValue) {}
void onAdiGsPointerChange(unsigned int newValue){}
void onAdiLocBarChange(unsigned int newValue) {}
void onAdiBubbleChange(unsigned int newValue) {}
void onAdiGsBarChange(unsigned int newValue) {}
void onAltPneuFlagChange(unsigned int newValue) {}
void onAltPressureDrum0CntChange(unsigned int newValue) {}
void onAltPressureDrum1CntChange(unsigned int newValue) {}
void onAltPressureDrum2CntChange(unsigned int newValue) {}
void onAltPressureDrum3CntChange(unsigned int newValue) {}
void onAlt10000FtCntChange(unsigned int newValue) {}
void onAlt1000FtCntChange(unsigned int newValue) {}
void onAlt100FtCntChange(unsigned int newValue) {}
void onAlt100FtPtrChange(unsigned int newValue) {}
void onAirspeedChange(unsigned int newValue) {}
void onAdiBankChange(unsigned int newValue) {}
void onAdiPitchChange(unsigned int newValue) {}
void onAoaValueChange(unsigned int newValue) {}
void onVviChange(unsigned int newValue) {}

DcsBios::IntegerBuffer adiTurnrateBuffer(ADI_TURNRATE_GAUGE_ADDRESS , ADI_TURNRATE_GAUGE_MASK , ADI_TURNRATE_GAUGE_SHIFTBY , onAdiTurnrateChange);
DcsBios::IntegerBuffer adiAuxFlagBuffer(ADI_AUX_FLAG_GAUGE_ADDRESS , ADI_AUX_FLAG_GAUGE_MASK , ADI_AUX_FLAG_GAUGE_SHIFTBY , onAdiAuxFlagChange);
DcsBios::IntegerBuffer adiLocFlagBuffer(ADI_LOC_FLAG_GAUGE_ADDRESS , ADI_LOC_FLAG_GAUGE_MASK , ADI_LOC_FLAG_GAUGE_SHIFTBY , onAdiLocFlagChange);
DcsBios::IntegerBuffer adiOffFlagBuffer(ADI_OFF_FLAG_GAUGE_ADDRESS , ADI_OFF_FLAG_GAUGE_MASK , ADI_OFF_FLAG_GAUGE_SHIFTBY , onAdiOffFlagChange);
DcsBios::IntegerBuffer adiGsFlagBuffer(ADI_GS_FLAG_GAUGE_ADDRESS , ADI_GS_FLAG_GAUGE_MASK , ADI_GS_FLAG_GAUGE_SHIFTBY , onAdiGsFlagChange);
DcsBios::IntegerBuffer adiGsPointerBuffer(ADI_GS_POINTER_GAUGE_ADDRESS , ADI_GS_POINTER_GAUGE_MASK , ADI_GS_POINTER_GAUGE_SHIFTBY, onAdiGsPointerChange);
DcsBios::IntegerBuffer adiLocBarBuffer(ADI_LOC_BAR_GAUGE_ADDRESS , ADI_LOC_BAR_GAUGE_MASK , ADI_LOC_BAR_GAUGE_SHIFTBY , onAdiLocBarChange);
DcsBios::IntegerBuffer adiBubbleBuffer(ADI_BUBBLE_GAUGE_ADDRESS , ADI_BUBBLE_GAUGE_MASK , ADI_BUBBLE_GAUGE_SHIFTBY , onAdiBubbleChange);
DcsBios::IntegerBuffer adiGsBarBuffer(ADI_GS_BAR_GAUGE_ADDRESS , ADI_GS_BAR_GAUGE_MASK , ADI_GS_BAR_GAUGE_SHIFTBY , onAdiGsBarChange);
DcsBios::IntegerBuffer altPneuFlagBuffer(ALT_PNEU_FLAG_GAUGE_ADDRESS , ALT_PNEU_FLAG_GAUGE_MASK , ALT_PNEU_FLAG_GAUGE_SHIFTBY , onAltPneuFlagChange);
DcsBios::IntegerBuffer altPressureDrum0CntBuffer(ALT_PRESSURE_DRUM_0_CNT_GAUGE_ADDRESS , ALT_PRESSURE_DRUM_0_CNT_GAUGE_MASK , ALT_PRESSURE_DRUM_0_CNT_GAUGE_SHIFTBY , onAltPressureDrum0CntChange);
DcsBios::IntegerBuffer altPressureDrum1CntBuffer(ALT_PRESSURE_DRUM_1_CNT_GAUGE_ADDRESS , ALT_PRESSURE_DRUM_1_CNT_GAUGE_MASK , ALT_PRESSURE_DRUM_1_CNT_GAUGE_SHIFTBY , onAltPressureDrum1CntChange);
DcsBios::IntegerBuffer altPressureDrum2CntBuffer(ALT_PRESSURE_DRUM_2_CNT_GAUGE_ADDRESS , ALT_PRESSURE_DRUM_2_CNT_GAUGE_MASK , ALT_PRESSURE_DRUM_2_CNT_GAUGE_SHIFTBY , onAltPressureDrum2CntChange);
DcsBios::IntegerBuffer altPressureDrum3CntBuffer(ALT_PRESSURE_DRUM_3_CNT_GAUGE_ADDRESS , ALT_PRESSURE_DRUM_3_CNT_GAUGE_MASK , ALT_PRESSURE_DRUM_3_CNT_GAUGE_SHIFTBY , onAltPressureDrum3CntChange);
DcsBios::IntegerBuffer alt10000FtCntBuffer(ALT_10000_FT_CNT_GAUGE_ADDRESS , ALT_10000_FT_CNT_GAUGE_MASK , ALT_10000_FT_CNT_GAUGE_SHIFTBY , onAlt10000FtCntChange);
DcsBios::IntegerBuffer alt1000FtCntBuffer(ALT_1000_FT_CNT_GAUGE_ADDRESS , ALT_1000_FT_CNT_GAUGE_MASK , ALT_1000_FT_CNT_GAUGE_SHIFTBY , onAlt1000FtCntChange);
DcsBios::IntegerBuffer alt100FtCntBuffer(ALT_100_FT_CNT_GAUGE_ADDRESS , ALT_100_FT_CNT_GAUGE_MASK , ALT_100_FT_CNT_GAUGE_SHIFTBY , onAlt100FtCntChange);
DcsBios::IntegerBuffer alt100FtPtrBuffer(ALT_100_FT_PTR_GAUGE_ADDRESS , ALT_100_FT_PTR_GAUGE_MASK , ALT_100_FT_PTR_GAUGE_SHIFTBY , onAlt100FtPtrChange);
DcsBios::IntegerBuffer airspeedBuffer(SET_AIRSPEED_GAUGE_ADDRESS , SET_AIRSPEED_GAUGE_MASK , SET_AIRSPEED_GAUGE_SHIFTBY , onAirspeedChange);
DcsBios::IntegerBuffer adiBankBuffer(ADI_BANK_GAUGE_ADDRESS , ADI_BANK_GAUGE_MASK , ADI_BANK_GAUGE_SHIFTBY , onAdiBankChange);
DcsBios::IntegerBuffer adiPitchBuffer(ADI_PITCH_GAUGE_ADDRESS , ADI_PITCH_GAUGE_MASK , ADI_PITCH_GAUGE_SHIFTBY , onAdiPitchChange);
DcsBios::IntegerBuffer aoaValueBuffer(AOA_VALUE_GAUGE_ADDRESS , AOA_VALUE_GAUGE_MASK , AOA_VALUE_GAUGE_SHIFTBY , onAoaValueChange);
DcsBios::IntegerBuffer vviBuffer(VVI_GAUGE_ADDRESS , VVI_GAUGE_MASK , VVI_GAUGE_SHIFTBY , onVviChange);
DcsBios::IntegerBuffer mainPwrSwBuffer           (MAIN_PWR_SW_SELECTOR_ADDRESS,         MAIN_PWR_SW_SELECTOR_MASK,         MAIN_PWR_SW_SELECTOR_SHIFTBY,         onMainPwrSwChange);


void setup() {
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