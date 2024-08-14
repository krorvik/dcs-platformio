#define DCSBIOS_DEFAULT_SERIAL
#include <DcsBios.h>
#include <i2c.h>
#include <stepper.h>
#include <f16c_data.h>
#include <helpers.h>

const int bubble_min = 7;
const int bubble_max = 121;
int bubble = 32768;
int turnrate = 32768;
 
const int dev_xmin = 0;
const int dev_xmax = 128;

const unsigned int arrow = 0x25bc;
const unsigned int dot = 0x25cf;



// altimeter multipliers, long because summing these may result in > 16bit values
long alt_100_steps = 0;
long alt_1k_steps = 0;
long alt_10k_steps = 0;


ViperStepper        airspeed_stepper ( 720,    0,  720,  1600,  50000, 5);
ViperStepper        altimeter_stepper( 720,    0,  720,  1600,  50000, 5);
ViperStepper        aoa_stepper      (1010,    0, 1100,  1600,  50000, 10); // mid pos for aos is 22500 (raw dcs).
ViperStepper        vvi_stepper      (1010,    0, 1010,  1600,  50000, 10); // mid pos for vvi is 36200 (raw dcs)
ViperStepper        adi_pitch_stepper( 340,  0,  340, 1600,  5000, 5);
ViperStepper        adi_bank_stepper ( 200, 0,   200, 1600,  2000, 1);

// DCS Bios callback setup, this is where the instruments meet the callbacks - and the steppers are assigned
// Make sure we can no longer adjust needles after poweron, and if switched off, enable stepper adjust again

// Mach number
void onMachIndicatorChange(unsigned int newValue) {  dtostrf(calculate_mach(newValue), 4, 2, mach_chars); }

// bubble data
void onAdiTurnrateChange(unsigned int newValue) { turnrate = newValue;}
void onAdiBubbleChange(unsigned int newValue) { bubble = newValue; }

// Barometric pressure display
void onAltPressureDrum0CntChange(unsigned int newValue) { baro_digits[3] = '0' + translate_digit(newValue); }
void onAltPressureDrum1CntChange(unsigned int newValue) { baro_digits[2] = '0' + translate_digit(newValue); }
void onAltPressureDrum2CntChange(unsigned int newValue) { baro_digits[1] = '0' + translate_digit(newValue); }
void onAltPressureDrum3CntChange(unsigned int newValue) { baro_digits[0] = '0' + translate_digit(newValue); }
 
// Callbacks for altimeter display
void onAltPneuFlagChange(unsigned int newValue) {}


void onAlt10000FtCntChange(unsigned int newValue) { 
  alt_digits[0] = '0' + translate_digit(newValue); 
  alt_10k_steps = translate_digit(newValue) * 8 * 720 * 10; // hardcoded res, cause fsck it
}

void onAlt1000FtCntChange(unsigned int newValue)  { 
  alt_digits[1] = '0' + translate_digit(newValue); 
  alt_1k_steps = translate_digit(newValue) * 8 * 720; // hardcoded res, cause fsck it
}

void onAlt100FtCntChange(unsigned int newValue) {
  alt_digits[2] = '0' + translate_digit(newValue);
  // Calculate mods
  alt_digits[3] = '0' + map(newValue % (65535 / 10), 0, 6554, 0, 10);
  alt_100_steps = (long) map(newValue, 0, 65535, 0, 8 * 720);  // hardcoded res, cause fsck it
}

// Stepper callbacks
// void onAlt100FtPtrChange(unsigned int newValue) { altimeter_stepper.moveToContinuous(newValue); }

void onAirspeedChange(unsigned int newValue)    { airspeed_stepper.moveToBounded(translate_ias(newValue) * 11.6); }
void onAdiBankChange(unsigned int newValue)     { adi_bank_stepper.moveToContinuous(newValue - 32768); }
void onAdiPitchChange(unsigned int newValue)    { adi_pitch_stepper.moveToBounded(newValue - 32768); }
void onAoaValueChange(unsigned int newValue)    { aoa_stepper.moveToBounded(translate_indexer(newValue, 20000)); }
void onVviChange(unsigned int newValue)         { vvi_stepper.moveToBounded(translate_indexer(newValue, 36200)); }


DcsBios::IntegerBuffer adiTurnrateBuffer        (ADI_TURNRATE_GAUGE_ADDRESS , ADI_TURNRATE_GAUGE_MASK , ADI_TURNRATE_GAUGE_SHIFTBY , onAdiTurnrateChange);
DcsBios::IntegerBuffer adiBubbleBuffer          (ADI_BUBBLE_GAUGE_ADDRESS , ADI_BUBBLE_GAUGE_MASK , ADI_BUBBLE_GAUGE_SHIFTBY , onAdiBubbleChange);
DcsBios::IntegerBuffer altPneuFlagBuffer        (ALT_PNEU_FLAG_GAUGE_ADDRESS , ALT_PNEU_FLAG_GAUGE_MASK , ALT_PNEU_FLAG_GAUGE_SHIFTBY , onAltPneuFlagChange);
DcsBios::IntegerBuffer altPressureDrum0CntBuffer(ALT_PRESSURE_DRUM_0_CNT_GAUGE_ADDRESS , ALT_PRESSURE_DRUM_0_CNT_GAUGE_MASK , ALT_PRESSURE_DRUM_0_CNT_GAUGE_SHIFTBY , onAltPressureDrum0CntChange);
DcsBios::IntegerBuffer altPressureDrum1CntBuffer(ALT_PRESSURE_DRUM_1_CNT_GAUGE_ADDRESS , ALT_PRESSURE_DRUM_1_CNT_GAUGE_MASK , ALT_PRESSURE_DRUM_1_CNT_GAUGE_SHIFTBY , onAltPressureDrum1CntChange);
DcsBios::IntegerBuffer altPressureDrum2CntBuffer(ALT_PRESSURE_DRUM_2_CNT_GAUGE_ADDRESS , ALT_PRESSURE_DRUM_2_CNT_GAUGE_MASK , ALT_PRESSURE_DRUM_2_CNT_GAUGE_SHIFTBY , onAltPressureDrum2CntChange);
DcsBios::IntegerBuffer altPressureDrum3CntBuffer(ALT_PRESSURE_DRUM_3_CNT_GAUGE_ADDRESS , ALT_PRESSURE_DRUM_3_CNT_GAUGE_MASK , ALT_PRESSURE_DRUM_3_CNT_GAUGE_SHIFTBY , onAltPressureDrum3CntChange);
DcsBios::IntegerBuffer alt10000FtCntBuffer      (ALT_10000_FT_CNT_GAUGE_ADDRESS , ALT_10000_FT_CNT_GAUGE_MASK , ALT_10000_FT_CNT_GAUGE_SHIFTBY , onAlt10000FtCntChange);
DcsBios::IntegerBuffer alt1000FtCntBuffer       (ALT_1000_FT_CNT_GAUGE_ADDRESS , ALT_1000_FT_CNT_GAUGE_MASK , ALT_1000_FT_CNT_GAUGE_SHIFTBY , onAlt1000FtCntChange);
DcsBios::IntegerBuffer alt100FtCntBuffer        (ALT_100_FT_CNT_GAUGE_ADDRESS , ALT_100_FT_CNT_GAUGE_MASK , ALT_100_FT_CNT_GAUGE_SHIFTBY , onAlt100FtCntChange);
// DcsBios::IntegerBuffer alt100FtPtrBuffer        (ALT_100_FT_PTR_GAUGE_ADDRESS , ALT_100_FT_PTR_GAUGE_MASK , ALT_100_FT_PTR_GAUGE_SHIFTBY , onAlt100FtPtrChange);
DcsBios::IntegerBuffer airspeedBuffer           (AIRSPEED_GAUGE_ADDRESS , AIRSPEED_GAUGE_MASK , AIRSPEED_GAUGE_SHIFTBY , onAirspeedChange);
DcsBios::IntegerBuffer adiBankBuffer            (ADI_BANK_GAUGE_ADDRESS , ADI_BANK_GAUGE_MASK , ADI_BANK_GAUGE_SHIFTBY , onAdiBankChange);
DcsBios::IntegerBuffer adiPitchBuffer           (ADI_PITCH_GAUGE_ADDRESS , ADI_PITCH_GAUGE_MASK , ADI_PITCH_GAUGE_SHIFTBY , onAdiPitchChange);
DcsBios::IntegerBuffer aoaValueBuffer           (AOA_VALUE_GAUGE_ADDRESS , AOA_VALUE_GAUGE_MASK , AOA_VALUE_GAUGE_SHIFTBY , onAoaValueChange);
DcsBios::IntegerBuffer vviBuffer                (VVI_GAUGE_ADDRESS , VVI_GAUGE_MASK , VVI_GAUGE_SHIFTBY , onVviChange);
DcsBios::IntegerBuffer machIndicatorBuffer      (MACH_INDICATOR_GAUGE_ADDRESS , MACH_INDICATOR_GAUGE_MASK , MACH_INDICATOR_GAUGE_SHIFTBY , onMachIndicatorChange);

// Encoder on altimeter - no bueno in current dcs bios :(
// DcsBios::RotaryEncoder altBaroSetKnb("ALT_BARO_SET_KNB", "-3200", "+3200", 16, 17);

// TODO: Convert from u8g2 to adafruit gfx
void display_bubble() {
  switchBus(1);
  display.clearDisplay();
  display.dim(true);
  // Static pattern, set drawcolor to default, since we change it later in this func
  display.fillRect(0,0, 128, 18, WHITE);
  display.fillRect(2, 19, 24, 6, WHITE);
  display.fillRect(52, 19, 24, 6, WHITE);
  display.fillRect(104, 19, 24, 6, WHITE);

  // Draw turn box
  display.fillRect(map(turnrate, 22768, 42768, 0, 104), 26, 24, 6, WHITE);
  
  // Set drawColor to 0, 1 is default - invert color for ball
  //display.setDrawColor(0);
  // Static lines for ball center
  display.drawFastVLine(55, 0, 18, SSD1306_BLACK);
  display.drawFastVLine(73, 0, 18, SSD1306_BLACK);
  // Actual ball
  display.fillCircle(map(bubble, 0, 65535, bubble_min, bubble_max),9,7, SSD1306_BLACK);
  display.display();
}

// Hook up stuff to do at end of dcs bios updates (all values are set at this point)
void onUpdateCounterChange(unsigned int newValue) { 
  // stepper->moveTo((int32_t) (alt_100_steps + alt_1k_steps  + alt_10k_steps));
  altimeter_stepper.moveTo(alt_100_steps + alt_1k_steps  + alt_10k_steps);
}
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);

void test() {
  airspeed_stepper.moveToBounded(65535);
  delay(1000);
  altimeter_stepper.moveToBounded(65535);
  delay(1000);
  airspeed_stepper.moveToBounded(0);
  delay(1000);
  altimeter_stepper.moveToBounded(0);
}

void setup() {
  delay(2000);
  stepper_init();  
  display_init_mux(0, 4);
  DcsBios::setup();
}

void display_all() {
  write_display_mux(2, String(mach_chars), 25, 5, 3);
  write_display_mux(3, String(baro_digits), 28, 5, 3);
  write_display_mux(4, alt_checkered(), 0, 5, 4);
  display_bubble();
}

void loop() {
  stepper_loop();  
  if (initAllowed()) {
    write_display_mux(2, String(getStepperID() ), 55, 0, 4);
  } else {
    display_all();
    DcsBios::loop();
  }
  
}