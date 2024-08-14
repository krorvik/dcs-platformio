#define DCSBIOS_RS485_SLAVE 87
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include "f16c_data.h"
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "Button2.h"

#define DISPLAY_WIDTH 128 // OLED display width, in pixels
#define DISPLAY_HEIGHT 32 // OLED display height, in pixels
#define DISPLAY_ADDRESS 0x3C
#define MUX_ADDRESS 0x70
#define OLED_RESET     -1
#define BUBBLE_MIN 7
#define BUBBLE_MAX 121
#define TURNRATE_MIN 0
#define TURNRATE_MAX 104
#define BARO_ALT_MODE_PIN 5

#define m15_limit 49000
#define m1_limit 60413
#define on_limit 62782

// needed constants
const unsigned int m1_ratio = (m1_limit - m15_limit) * 2;
const unsigned int on_ratio = (on_limit - m1_limit) * 2;

// Var data
unsigned int bubble = 32768;
unsigned int turnrate = 32768;
char alt_digits[6] = { '0', '0', '0', '0', '0', '\0'};
char baro_digits[7] = {'E', ' ', '2', '9', '9', '2', '\0'};
char mach_chars[5] = {'0', '.', '0', '0', '\0'};
unsigned int baro_alt_presses = 0;

Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, OLED_RESET);
Button2 baro_mode_button(BARO_ALT_MODE_PIN);

// Switch the bus on the tmux - used if we request a bus in 
void switchBus(uint8_t busid) {
  Wire.beginTransmission(MUX_ADDRESS);
  Wire.write(1 << busid);
  Wire.endTransmission();
}

void write_display(unsigned int display_bus, String data, int x = 0, int y = 0, int textsize = 3) {
  switchBus(display_bus);
  display.clearDisplay();
  display.setTextSize(textsize);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(x,y);
  display.println(data);
  display.display();
}

void display_bubble() {
  switchBus(6);
  display.clearDisplay();
  // Static pattern, set drawcolor to default, since we change it later in this func
  display.fillRect(0,0, 128, 18, SSD1306_WHITE);
  display.fillRect(2, 19, 24, 6, SSD1306_WHITE);
  display.fillRect(52, 19, 24, 6, SSD1306_WHITE);
  display.fillRect(104, 19, 24, 6, SSD1306_WHITE);

  // Static lines for ball center
  display.drawFastVLine(55, 0, 18, SSD1306_BLACK);
  display.drawFastVLine(73, 0, 18, SSD1306_BLACK);
  // Actual ball
  display.fillCircle(map(bubble, 0, 65535, BUBBLE_MIN, BUBBLE_MAX),9,7, SSD1306_BLACK);
  // Draw turn box
  display.fillRect(map(turnrate, 22768, 42768, TURNRATE_MIN, TURNRATE_MAX), 26, 24, 6, SSD1306_WHITE);
  display.display();
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


float calculate_mach(unsigned int value) {  
  if (value >= on_limit) {
    return 0.5;
  } else if (value >= m1_limit) {
    return 0.5 + (on_limit - (float) value) / on_ratio;
  } else {
    return 1 + (m1_limit - (float) value) / m1_ratio;
  }
}

char* alt_checkered() {
  char *tmp_digits = alt_digits;
  if (alt_digits[0] == '0') {
    tmp_digits[0] = 0xB0;
    if (alt_digits[1] == '0' ) { tmp_digits[1] = 0xB0;}
  }
  return tmp_digits;
}

void display_all() {
  write_display(4, String(mach_chars), 25, 5, 3);
  write_display(5, String(baro_digits), 0, 5, 3);
  write_display(3, alt_checkered(), 0, 5, 4);
  display_bubble();
}

// bubble data
void onAdiTurnrateChange(unsigned int newValue) { turnrate = newValue;}
void onAdiBubbleChange(unsigned int newValue) { bubble = newValue; }

// Barometric pressure display
void onAltPressureDrum0CntChange(unsigned int newValue) { baro_digits[5] = '0' + translate_digit(newValue); }
void onAltPressureDrum1CntChange(unsigned int newValue) { baro_digits[4] = '0' + translate_digit(newValue); }
void onAltPressureDrum2CntChange(unsigned int newValue) { baro_digits[3] = '0' + translate_digit(newValue); }
void onAltPressureDrum3CntChange(unsigned int newValue) { baro_digits[2] = '0' + translate_digit(newValue); }
 
// Callbacks for altimeter display
void onAltPneuFlagChange(unsigned int newValue) {
  if(newValue == 0) {
    baro_digits[0] = 'E';
  } else {
    baro_digits[0] = 'P';
  }
}

void press_baro_mode(Button2 &button) {
  // Switch directions for each press, and send bios msg for dir
  char* direction;
  if (baro_alt_presses % 2 == 0) {
    direction = "0";
  } else {
    direction = "2";
  }
  // emit
  sendDcsBiosMessage("ALT_MODE_LV", direction);
  baro_alt_presses += 1;
}

void release_baro_mode(Button2 &button) {
  // Center button
  sendDcsBiosMessage("ALT_MODE_LV", "1");
}

void onAlt10000FtCntChange(unsigned int newValue) { 
  alt_digits[0] = '0' + translate_digit(newValue); 
}

void onAlt1000FtCntChange(unsigned int newValue)  { 
  alt_digits[1] = '0' + translate_digit(newValue); 
}

void onAlt100FtCntChange(unsigned int newValue) {
  alt_digits[2] = '0' + translate_digit(newValue);
  // Calculate mods
  alt_digits[3] = '0' + map(newValue % (65535 / 10), 0, 6554, 0, 10);
}

void onMachIndicatorChange(unsigned int newValue) {  dtostrf(calculate_mach(newValue), 4, 2, mach_chars); }

void onUpdateCounterChange(unsigned int newValue) {
  display_all();
}

DcsBios::RotaryEncoderT<POLL_EVERY_TIME, DcsBios::FOUR_STEPS_PER_DETENT> altBaroSetKnb("ALT_BARO_SET_KNB", "-1600", "+1600", 3,4);

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
DcsBios::IntegerBuffer machIndicatorBuffer      (MACH_INDICATOR_GAUGE_ADDRESS , MACH_INDICATOR_GAUGE_MASK , MACH_INDICATOR_GAUGE_SHIFTBY , onMachIndicatorChange);
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);

void setup() {  
  DcsBios::setup();
  // Init button
  baro_mode_button.begin(BARO_ALT_MODE_PIN);
  baro_mode_button.setPressedHandler(press_baro_mode);
  baro_mode_button.setReleasedHandler(release_baro_mode);
  // Init displays
  Wire.begin();
  for (unsigned int i = 3; i<=6; i++) {
    switchBus(i); 
    display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS);
  }
  display_all();
}

void loop() {
  // put your main code here, to run repeatedly:  
  baro_mode_button.loop();
  DcsBios::loop();
}
