#define DCSBIOS_RS485_SLAVE 42
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include <Adafruit_SSD1306.h>
#include "f16c_data.h"

// Start with needles in center (will be moved by sim)
long deviation = 32;
unsigned int loc_pos = 64;  // moves along x
unsigned int gs_pos = 32;   // moves along y
bool loc_visible = true;
bool gs_visible = true;
bool gs_flag = true;
bool loc_flag = true;
bool off_flag = true;
bool aux_flag = true;

// TODO:
int getLocPos(unsigned int newValue) {

}
//TODO:
int getGsPos(unsigned int newValue) {
    
}

void onAdiGsFlagChange(unsigned int newValue) { gs_flag = (newValue > 35535); }
void onAdiLocFlagChange(unsigned int newValue) { loc_flag = (newValue > 35535); }
void onAdiOffFlagChange(unsigned int newValue) { off_flag = (newValue > 35535); }
void onAdiAuxFlagChange(unsigned int newValue) { aux_flag = (newValue > 35535); }
void onAdiGsPointerChange(unsigned int newValue){ deviation = map(newValue, 0, 65535, 64, 0); }

void onAdiGsBarChange(unsigned int newValue) { 
  gs_pos = map(newValue, 17263, 48272, 0, 64); 
  gs_visible = newValue <= 48272 or newValue >= 17263;
}
void onAdiLocBarChange(unsigned int newValue) {
  loc_pos = map(newValue, 13300, 52235, 32, 96); 
  loc_visible = newValue >= 13300 or newValue <= 52235;
}

DcsBios::IntegerBuffer adiGsFlagBuffer(ADI_GS_FLAG_GAUGE_MASK , ADI_GS_FLAG_GAUGE_MASK, ADI_GS_FLAG_GAUGE_SHIFTBY, onAdiGsFlagChange);
DcsBios::IntegerBuffer adiLocFlagBuffer(ADI_LOC_FLAG_GAUGE_ADDRESS, ADI_LOC_FLAG_GAUGE_MASK, ADI_LOC_FLAG_GAUGE_SHIFTBY, onAdiLocFlagChange);
DcsBios::IntegerBuffer adiOffFlagBuffer(ADI_OFF_FLAG_GAUGE_ADDRESS, ADI_OFF_FLAG_GAUGE_MASK, ADI_OFF_FLAG_GAUGE_SHIFTBY, onAdiOffFlagChange);
DcsBios::IntegerBuffer adiAuxFlagBuffer(ADI_AUX_FLAG_GAUGE_ADDRESS, ADI_AUX_FLAG_GAUGE_MASK, ADI_AUX_FLAG_GAUGE_SHIFTBY, onAdiAuxFlagChange);
DcsBios::IntegerBuffer adiGsBarBuffer(ADI_GS_BAR_GAUGE_ADDRESS, ADI_GS_BAR_GAUGE_MASK, ADI_GS_BAR_GAUGE_SHIFTBY, onAdiGsBarChange);
DcsBios::IntegerBuffer adiLocBarBuffer(ADI_LOC_BAR_GAUGE_ADDRESS, ADI_LOC_BAR_GAUGE_MASK, ADI_LOC_BAR_GAUGE_SHIFTBY, onAdiLocBarChange);
DcsBios::IntegerBuffer adiGsPointerBuffer(ADI_GS_POINTER_GAUGE_ADDRESS, ADI_GS_POINTER_GAUGE_MASK, ADI_GS_POINTER_GAUGE_SHIFTBY, onAdiGsPointerChange);

//Define SPI Connections, in order on display:
#define OLED_CS    3
#define OLED_DC    4
#define OLED_RESET 5
#define OLED_MOSI  6
#define OLED_CLK   7

//Remove comments (//) if using default SPI connection:
Adafruit_SSD1306 display(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void draw_deviation_scale() {
  display.drawFastHLine(32,16, 3, WHITE);
  display.drawFastHLine(32,32, 3, WHITE);
  display.drawFastHLine(32,48, 3, WHITE);
}

void draw_ils_back() {
  display.drawCircle(64,32, 3, WHITE);
  display.drawFastVLine(32,0,3, WHITE);
  display.drawFastHLine(32,0,3, WHITE);
  display.drawFastVLine(96,0,3, WHITE);
  display.drawFastHLine(94,0,3, WHITE);
  display.drawFastVLine(32,61,3, WHITE);
  display.drawFastHLine(32,63,3, WHITE);
  display.drawFastVLine(96,61,3, WHITE);
  display.drawFastHLine(94,63,3, WHITE);
}

void draw_ils_needles() {
    display.drawFastVLine(loc_pos, 5, 54, WHITE);
    display.drawFastHLine(37, gs_pos, 54 , WHITE);
}

void draw_adi_flags() {
  if (gs_flag) {
    display.setCursor(1,1);  
    display.drawRect(0,0,14,10, WHITE);
    display.print("GS");
  }
  if (loc_flag) {
    display.setCursor(109,1);
    display.drawRect(108,0,20,10, WHITE);
    display.print("LOC");
  }
  if (off_flag) {
    display.setCursor(1,53);
    display.drawRect(0,52,20,10, WHITE);
    display.print("OFF");
  }
  if (aux_flag) {
    display.setCursor(109,53);
    display.drawRect(108,52,20,10, WHITE);
    display.print("AUX");
  }
}

// This one should simply draw a 
void draw_deviation() {
  display.fillCircle(28, deviation, 2, WHITE);
}

void display_loop() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);  
  if (loc_visible and gs_visible) {
    draw_ils_back();
    draw_deviation_scale();
    draw_deviation();
    draw_ils_needles();
  }
  draw_adi_flags();  
  display.display();
}

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  //Initialize with the I2C address 0x3C.
  display_loop();
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
  display_loop();
}