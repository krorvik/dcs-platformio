#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>
#include <i2c.h>
#include <stepper.h>
#include <f16c_data.h>
#include <helpers.h>
#include <adi_displays.h>
#include <adi_flags.h>

int deviation = 32768;
int bubble = 32768;
int turnrate = 32768;

ViperStepper        airspeed_stepper ( 720,    0,  720,  800,  10000, 5);
ViperStepper        altimeter_stepper( 720,    0,  720,  800,  10000, 5);
ViperStepper        aoa_stepper      (1010,    0, 1010, 1000,  5000, 10); // mid pos for aos is 22500 (raw dcs).
ViperStepper        vvi_stepper      (1010,    0, 1010, 1000,  5000, 10); // mid pos for vvi is 36200 (raw dcs)
ViperStepper        adi_pitch_stepper( 340,  170,  -170, 1000,  2000, 5);
ViperStepper        adi_bank_stepper ( 200, -100,  100,  200,  1500, 1);

// DCS Bios callback setup, this is where the instruments meet the callbacks - and the steppers are assigned
// Make sure we can no longer adjust needles after poweron, and if switched off, enable stepper adjust again

// Mach number
void onMachIndicatorChange(unsigned int newValue) {  dtostrf(calculate_mach(newValue), 4, 2, mach_chars); }

// ADI Flags
/*
void onAdiLocFlagChange(unsigned int newValue) { set_servo_mux(0, newValue, 5); }
void onAdiAuxFlagChange(unsigned int newValue) { set_servo_mux(1, newValue, 5); }
void onAdiOffFlagChange(unsigned int newValue) { set_servo_mux(2, newValue, 5); }
void onAdiGsFlagChange(unsigned int newValue)  { set_servo_mux(3, newValue, 5); }
*/

// Deviation/bubble data
void onAdiGsPointerChange(unsigned int newValue){ deviation = newValue; }
void onAdiTurnrateChange(unsigned int newValue) { turnrate = newValue;}
void onAdiBubbleChange(unsigned int newValue) { bubble = newValue; }

// Barometric pressure display
void onAltPressureDrum0CntChange(unsigned int newValue) { baro_digits[3] = '0' + translate_digit(newValue); }
void onAltPressureDrum1CntChange(unsigned int newValue) { baro_digits[2] = '0' + translate_digit(newValue); }
void onAltPressureDrum2CntChange(unsigned int newValue) { baro_digits[1] = '0' + translate_digit(newValue); }
void onAltPressureDrum3CntChange(unsigned int newValue) { baro_digits[0] = '0' + translate_digit(newValue); }

// Callbacks for altimeter display
void onAltPneuFlagChange(unsigned int newValue) {}
void onAlt10000FtCntChange(unsigned int newValue) { alt_digits[0] = '0' + translate_digit(newValue); }
void onAlt1000FtCntChange(unsigned int newValue)  { alt_digits[1] = '0' + translate_digit(newValue); }

void onAlt100FtCntChange(unsigned int newValue) {
  alt_digits[2] = '0' + translate_digit(newValue);
  // Calculate mods
  alt_digits[3] = '0' + map(newValue % (65535 / 10), 0, 6554, 0, 10);
}

// Stepper callbacks
void onAlt100FtPtrChange(unsigned int newValue) {altimeter_stepper.moveToContinuous(newValue); }
void onAirspeedChange(unsigned int newValue)    { airspeed_stepper.moveToBounded(translate_ias(newValue)); }
void onAdiBankChange(unsigned int newValue)     { adi_bank_stepper.moveToContinuous(newValue); }
void onAdiPitchChange(unsigned int newValue)    { adi_pitch_stepper.moveToBounded(newValue); }
void onAoaValueChange(unsigned int newValue)    { aoa_stepper.moveToBounded(translate_indexer(newValue, 22500)); }
void onVviChange(unsigned int newValue)         { vvi_stepper.moveToBounded(translate_indexer(newValue, 36200)); }

DcsBios::IntegerBuffer adiTurnrateBuffer        (ADI_TURNRATE_GAUGE_ADDRESS , ADI_TURNRATE_GAUGE_MASK , ADI_TURNRATE_GAUGE_SHIFTBY , onAdiTurnrateChange);
// DcsBios::IntegerBuffer adiAuxFlagBuffer         (ADI_AUX_FLAG_GAUGE_ADDRESS , ADI_AUX_FLAG_GAUGE_MASK , ADI_AUX_FLAG_GAUGE_SHIFTBY , onAdiAuxFlagChange);
// DcsBios::IntegerBuffer adiLocFlagBuffer         (ADI_LOC_FLAG_GAUGE_ADDRESS , ADI_LOC_FLAG_GAUGE_MASK , ADI_LOC_FLAG_GAUGE_SHIFTBY , onAdiLocFlagChange);
// DcsBios::IntegerBuffer adiOffFlagBuffer         (ADI_OFF_FLAG_GAUGE_ADDRESS , ADI_OFF_FLAG_GAUGE_MASK , ADI_OFF_FLAG_GAUGE_SHIFTBY , onAdiOffFlagChange);
// DcsBios::IntegerBuffer adiGsFlagBuffer          (ADI_GS_FLAG_GAUGE_ADDRESS , ADI_GS_FLAG_GAUGE_MASK , ADI_GS_FLAG_GAUGE_SHIFTBY , onAdiGsFlagChange);
DcsBios::IntegerBuffer adiBubbleBuffer          (ADI_BUBBLE_GAUGE_ADDRESS , ADI_BUBBLE_GAUGE_MASK , ADI_BUBBLE_GAUGE_SHIFTBY , onAdiBubbleChange);
DcsBios::IntegerBuffer altPneuFlagBuffer        (ALT_PNEU_FLAG_GAUGE_ADDRESS , ALT_PNEU_FLAG_GAUGE_MASK , ALT_PNEU_FLAG_GAUGE_SHIFTBY , onAltPneuFlagChange);
DcsBios::IntegerBuffer altPressureDrum0CntBuffer(ALT_PRESSURE_DRUM_0_CNT_GAUGE_ADDRESS , ALT_PRESSURE_DRUM_0_CNT_GAUGE_MASK , ALT_PRESSURE_DRUM_0_CNT_GAUGE_SHIFTBY , onAltPressureDrum0CntChange);
DcsBios::IntegerBuffer altPressureDrum1CntBuffer(ALT_PRESSURE_DRUM_1_CNT_GAUGE_ADDRESS , ALT_PRESSURE_DRUM_1_CNT_GAUGE_MASK , ALT_PRESSURE_DRUM_1_CNT_GAUGE_SHIFTBY , onAltPressureDrum1CntChange);
DcsBios::IntegerBuffer altPressureDrum2CntBuffer(ALT_PRESSURE_DRUM_2_CNT_GAUGE_ADDRESS , ALT_PRESSURE_DRUM_2_CNT_GAUGE_MASK , ALT_PRESSURE_DRUM_2_CNT_GAUGE_SHIFTBY , onAltPressureDrum2CntChange);
DcsBios::IntegerBuffer altPressureDrum3CntBuffer(ALT_PRESSURE_DRUM_3_CNT_GAUGE_ADDRESS , ALT_PRESSURE_DRUM_3_CNT_GAUGE_MASK , ALT_PRESSURE_DRUM_3_CNT_GAUGE_SHIFTBY , onAltPressureDrum3CntChange);
DcsBios::IntegerBuffer alt10000FtCntBuffer      (ALT_10000_FT_CNT_GAUGE_ADDRESS , ALT_10000_FT_CNT_GAUGE_MASK , ALT_10000_FT_CNT_GAUGE_SHIFTBY , onAlt10000FtCntChange);
DcsBios::IntegerBuffer alt1000FtCntBuffer       (ALT_1000_FT_CNT_GAUGE_ADDRESS , ALT_1000_FT_CNT_GAUGE_MASK , ALT_1000_FT_CNT_GAUGE_SHIFTBY , onAlt1000FtCntChange);
DcsBios::IntegerBuffer alt100FtCntBuffer        (ALT_100_FT_CNT_GAUGE_ADDRESS , ALT_100_FT_CNT_GAUGE_MASK , ALT_100_FT_CNT_GAUGE_SHIFTBY , onAlt100FtCntChange);
DcsBios::IntegerBuffer alt100FtPtrBuffer        (ALT_100_FT_PTR_GAUGE_ADDRESS , ALT_100_FT_PTR_GAUGE_MASK , ALT_100_FT_PTR_GAUGE_SHIFTBY , onAlt100FtPtrChange);
DcsBios::IntegerBuffer airspeedBuffer           (AIRSPEED_GAUGE_ADDRESS , AIRSPEED_GAUGE_MASK , AIRSPEED_GAUGE_SHIFTBY , onAirspeedChange);
DcsBios::IntegerBuffer adiBankBuffer            (ADI_BANK_GAUGE_ADDRESS , ADI_BANK_GAUGE_MASK , ADI_BANK_GAUGE_SHIFTBY , onAdiBankChange);
DcsBios::IntegerBuffer adiPitchBuffer           (ADI_PITCH_GAUGE_ADDRESS , ADI_PITCH_GAUGE_MASK , ADI_PITCH_GAUGE_SHIFTBY , onAdiPitchChange);
DcsBios::IntegerBuffer aoaValueBuffer           (AOA_VALUE_GAUGE_ADDRESS , AOA_VALUE_GAUGE_MASK , AOA_VALUE_GAUGE_SHIFTBY , onAoaValueChange);
DcsBios::IntegerBuffer vviBuffer                (VVI_GAUGE_ADDRESS , VVI_GAUGE_MASK , VVI_GAUGE_SHIFTBY , onVviChange);
DcsBios::IntegerBuffer machIndicatorBuffer      (MACH_INDICATOR_GAUGE_ADDRESS , MACH_INDICATOR_GAUGE_MASK , MACH_INDICATOR_GAUGE_SHIFTBY , onMachIndicatorChange);

void setup() {
  stepper_init();  
  //servo_init(5);  // This locks up for some reason. Commented for now.
  display_init_mux(2, 4);
  adi_display_init(0, 1);
  DcsBios::setup();
}

void display_all() {
  display_deviation(deviation, 0);
  display_bubble(bubble, turnrate, 1);
  write_display_mux(2, String(mach_chars), 25, 5, 3);
  write_display_mux(3, String(baro_digits), 28, 5, 3);
  write_display_mux(4, alt_checkered(), 0, 5, 4);
}

void loop() {
  stepper_loop();  
  if (initAllowed()) {
    write_display_mux(2, String(getStepperID() ), 55, 0, 4) ;
    write_display_mux(0, "        ");
    write_display_mux(1, "        ");
    write_display_mux(3, "        ");
    write_display_mux(4, "        ");    
  } else {
    display_all();
    DcsBios::loop();
  }
  
}
