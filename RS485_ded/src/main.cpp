// Original credit:
// F-16 DED for DCS BIOS using 256x64 ER-OLED032-1G from BuyDisplay.com
// Final code by Wiggles5289 and a whole lotta cred to Fusion on the DCS Forums.

// My (Kenneth Rorvik) changes:
// Add outputs for right eyebrow lights.
// Another type of display - 192 x 64 https://www.aliexpress.com/item/4000870410380.html?spm=a2g0o.order_list.0.0.21ef18022PwgPe
// Remove unused import

// We use RS485, so this is way to go. Number must be unique across clients.
#define DCSBIOS_RS485_SLAVE 5
// Hardwired into RS485/nano/pro mini PCBs
#define TXENABLE_PIN 2
// No comment needed ;)
#include "DcsBios.h"
// Graphics
#include <U8g2lib.h>
#include <U8glib.h>
// HW SPI communications to display
#include <SPI.h>
#include "f16c_data.h"

// Yeah, started with 12 instead of A2, and that pin is one of the SPI pins. Moved to A2.
const int eyebrowpins[8] = { 6, 7, 8, 9, 10, A2, A0, A1};

// Display type determined by this one.
// Hardware SPI arduino nano, pin 11 is SDA (MOSI), 13 is SCK.
// parameters are rotation, and pins CS/SS, DC, RST.
U8G2_UC1609_SLG19264_2_4W_HW_SPI DED(U8G2_R0, /* cs=*/ 3, /* dc=*/ 4, /* reset=*/ 5);

// Initial lines of DED data, so we see it working. DCS will overwrite.
char* line1 = "           DCS";
char* line2 = "";
char* line3 = "      F-16C Blk 50";
char* line4 = "";
char* line5 = "          init";

// Current best font for DED by community here:
// https://forum.dcs.world/topic/261806-f16-ded-with-ssd1322-and-dcs-bios/page/3/
// Inverted characters are not yet implemented, but this PDF from DCS bios shows promise:
// https://github.com/DCSFlightpanels/dcs-bios/blob/master/Scripts/DCS-BIOS/doc/planes/DCS%20BIOS%20DED%20Processing%20Spec.pdf
// See in particular Appendix 1 - Format processing.

const u8g_fntpgm_uint8_t DEDfont16px[1148] U8G_FONT_SECTION("DEDfont16px") =
  "_\0\3\3\4\4\4\5\5\10\16\0\376\12\376\12\377\1\212\3\1\4_ \5\0\10\61!\14\224\32"
  "q$\207\212H,\222\0\42\10\66y\61DL\2#\22\227\31q\224$\222\213\222D$\221\134\224$"
  "\22\0$\20\326\371\260\204\62\212\210\252\225$\241\11E\0%\24\270\10q\206\22\231D\22\232Hu\31"
  "E\324$\302\11\0&\21\227\31qf\22\221D\66\223JL\22\221h\42'\6\62{\61\14(\15\264"
  "\372\260$\42\211H\67\221L\0)\15\264\372\60d\42\231H\27\221D\4*\20\310\10\61~\60\205h"
  "s\300\214\24:<\20+\12V\71\261\204\42\223P\4,\10C\373\60,\24\0-\6\26Y\61\14."
  "\6#\33\61\14/\16\246\11\61\325\204\62\241L(\23\12\1\60\16\226\32q(\42JS\245B\222P"
  "\0\61\11\225\31\361D\207\230>\62\14\226\31q(\42\222P\246\243\1\63\17\226\31q(\42\222P\64"
  "%\221$\24\0\64\20\227\31q\244*\42\211H\242$\71J%\0\65\15\226\31\61\16A\305\252P&"
  "!\1\66\15\226\31\261fj\25\21'\11\5\0\67\15\226\31\61\214\62\241L(\23\252\1\70\20\226\31"
  "q(\42REB\221\224H\22\12\0\71\15\226\31q(\42N\222\232\332\10\0:\10s\33\61\354 "
  "\3;\12\223\373\60\354 \13\5\0<\11\226\31\61e\272\352\0=\7\66I\61\314\6>\11\226\31\61"
  "\244\272\351\10\77\16\226\31q(\42\222LQ\16\23\212\0@\17\230\30q,B&\213\204\42!\331\1"
  "\7A\14\226\31\261d\24\21\323a\304$B\14\226\31\61*\42\246\213\210\351\2C\15\226\31q(\42"
  "FM$\11\5\0D\14\226\31\61H\22%\276HH\0E\14\226\31\61\16A\305\212P\321\0F\14"
  "\226\31\61\16A\305\212P#\0G\14\226\31q(\42F\225\22IRH\13\226\31\61D\234\16#N"
  "\2I\11\224\32\61(\42}!J\13\226\31\61\365H\42I(\0K\15\226\31\61D,ZH\22\235"
  "H\2L\11\226\31\61\204\372\321\0M\20\227\31\61d\264J%B\211P\42\64\66\1N\15\227\31\61"
  "d\264\222\205b\252\261\11O\13\226\31q(\42~\222P\0P\14\226\31\61*\42\246\213P#\0Q"
  "\14\266\371p(\42~\222\20\245\2R\14\226\31\61*\42\246\213&&\1S\14\226\31q(\42\252V"
  "\222\204\2T\11\226\31\61LB\375\4U\12\226\31\61D\374IB\1V\13\226\31\61D\374$\241\211"
  "\0W\21\227\31\61d\134\42\224\10%\242I\42\222H\0X\17\226\31\61D$\211$&\224E\224\230"
  "\4Y\14\226\31\61D\234$\64\241&\0Z\12\226\31\61\214j:\12\15[\11\304\352\60L\372'\2"
  "\134\27\230\30\61d\22ID%\42\71L\42*\21\221D&\221ID\0]\11\304\352\60H\372'\3"
  "^\11\66\231\261d\24\221\0_\7\30\350\60\16\1`\10\64\232\61F\62\1a\16\267\31\361\242\263\312"
  "\35\303\245\66\215\1b\14\226\31\61\204\302\212\210\247\13\0c\14v\31q(\42\242&\11\5\0d\12"
  "\226\31\61UN<I\12e\14v\31q(\42\322a(\245\0f\14\226\31\261(B\65\213P\33\0"
  "g\14\246\351pN<I\212\302\13\0h\13\226\31\61\204\302\212\210O\2i\13\246\31\261\204r\10Q"
  "O\6j\12\325\351\360\224i\372\315\2k\15\226\31\61\204J,\22\222D\211$l\11\226\31\61\210\372"
  "\311\0m\21w\31\61,\222\10%B\211P\42\224\10M\0n\11v\31\61*\42>\11o\11Cm"
  "qR$)\0p\14\246\351\60*\42\236.BE\0q\12\246\351pN<I\212\32r\13v\31\61"
  "D\24\233P#\0s\12v\31q\216R\252\360\2t\13\226\31q\204\62\213PW\2u\11v\31\61"
  "D|\222\24v\13v\31\61D<Ih\42\0w\21w\31\61d\224\10%B\211P\42\232$\22\0"
  "x\15v\31\61D$\11MF\21\221\4y\24\247\350pD\22\221D$\21ID\22\221\210*\24\321"
  "\0z\11v\31\61\214\62\35\15{\14\265\371\360D\62%E\65E\1|\7\302\353\60>\14}\15\265"
  "\372\60\204\62E%\65%\31\0~\12\70xqf\23\311l\2\0\0\0\4\377\377\0";


// DED Callbacks called from DCS bios simply set values in variables.
void onDedLine1Change(char* newValue) {
  line1 = newValue;
}
void onDedLine2Change(char* newValue) {
  line2 = newValue;
}
void onDedLine3Change(char* newValue) {
  line3 = newValue;
}
void onDedLine4Change(char* newValue) {
  line4 = newValue;
}
void onDedLine5Change(char* newValue) {
  line5 = newValue;
}

void drawDED() {
  DED.firstPage();
  do {
    DED.drawStr(0, 12.8, line1);
    DED.drawStr(0, 25.6, line2);
    DED.drawStr(0, 38.4, line3);
    DED.drawStr(0, 51.2, line4);
    DED.drawStr(0, 64, line5);
  } while ( DED.nextPage() );
  DED.updateDisplay();
}

// DCS bios update callback - draws DED display.
void onUpdateCounterChange(unsigned int newValue) {
  drawDED();
}

// Register callbacks for DED
DcsBios::StringBuffer<25> dedLine1Buffer(DED_LINE_1_DISPLAY_ADDRESS, onDedLine1Change);
DcsBios::StringBuffer<25> dedLine2Buffer(DED_LINE_2_DISPLAY_ADDRESS, onDedLine2Change);
DcsBios::StringBuffer<25> dedLine3Buffer(DED_LINE_3_DISPLAY_ADDRESS, onDedLine3Change);
DcsBios::StringBuffer<25> dedLine4Buffer(DED_LINE_4_DISPLAY_ADDRESS, onDedLine4Change);
DcsBios::StringBuffer<25> dedLine5Buffer(DED_LINE_5_DISPLAY_ADDRESS, onDedLine5Change);

// Use the DCS bios update counter to trigger all changes
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);

// Add right eyebrow lights, pins D6-D10, D12, A0-A1. Simple DCS Bios LEDs to pins.
DcsBios::LED LIGHT_ENGINE ( LIGHT_ENGINE_LED_ADDRESS ,  LIGHT_ENGINE_LED_MASK , eyebrowpins[0]);
DcsBios::LED LIGHT_ENG_FIRE ( LIGHT_ENG_FIRE_LED_ADDRESS ,  LIGHT_ENG_FIRE_LED_MASK , eyebrowpins[1]);
DcsBios::LED LIGHT_HYD_OIL_PRESS ( LIGHT_HYD_OIL_PRESS_LED_ADDRESS ,  LIGHT_HYD_OIL_PRESS_LED_MASK , eyebrowpins[2]);
DcsBios::LED LIGHT_DBU_ON ( LIGHT_DBU_ON_LED_ADDRESS ,  LIGHT_DBU_ON_LED_MASK , eyebrowpins[3]);
DcsBios::LED LIGHT_FLCS ( LIGHT_FLCS_LED_ADDRESS ,  LIGHT_FLCS_LED_MASK , eyebrowpins[4]);
DcsBios::LED LIGHT_TO_LDG_CONFIG ( LIGHT_TO_LDG_CONFIG_LED_ADDRESS ,  LIGHT_TO_LDG_CONFIG_LED_MASK , eyebrowpins[5]);
DcsBios::LED LIGHT_OXY_LOW ( LIGHT_OXY_LOW_LED_ADDRESS ,  LIGHT_OXY_LOW_LED_MASK , eyebrowpins[6]);
DcsBios::LED LIGHT_CANOPY ( LIGHT_CANOPY_LED_ADDRESS ,  LIGHT_CANOPY_LED_MASK , eyebrowpins[7]);

void setup() {
  // Eyebrow lights init, light up a couple of seconds
  for (int index = 0; index < 8; index++) {
    pinMode(eyebrowpins[index], OUTPUT);
    digitalWrite(eyebrowpins[index], HIGH);
  }

  // init DED
  DED.begin();
  DED.clearBuffer();
  DED.setFont(DEDfont16px);
  drawDED();

  delay(2000);
  // Eyebrow Lights off
  for (int index = 0; index < 8; index++) {
    digitalWrite(eyebrowpins[index], LOW);
  }
  // Needs no comment? ;)
  DcsBios::setup();
}

void loop() {
  // Draw display

  // And DCS bios loop.
  DcsBios::loop();
}
