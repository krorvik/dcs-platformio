#define DCSBIOS_RS485_SLAVE 90
#define TXENABLE_PIN 2
#include <DcsBios.h>
#include <TM1637TinyDisplay.h>
#include <TM1637TinyDisplay6.h>
#include "f16c_data.h"

#define FREQ_CLK 3
#define FREQ_DIO 4
#define CHAN_CLK 5
#define CHAN_DIO 6

TM1637TinyDisplay chan_display(CHAN_CLK, CHAN_DIO);
TM1637TinyDisplay6 freq_display(FREQ_CLK, FREQ_DIO);

void displays_init() {
  chan_display.setBrightness(BRIGHT_HIGH);
  freq_display.setBrightness(BRIGHT_HIGH);
  chan_display.clear();
  freq_display.clear();
  chan_display.showNumber(88);
  freq_display.showNumber(888.888);
  delay(3000);
  chan_display.clear();
  freq_display.clear();
}

void onUhfChanDispChange(char* newValue) {
  if (strcmp(newValue, "**") == 0) {
    chan_display.showString("88");
  } else {
    int chanval = atoi(newValue);
    if (chanval > 0) {      
      chan_display.showNumber(chanval);
    } else {
      chan_display.clear();
    }
  }

}
void onUhfFreqDispChange(char* newValue) {
  if (strcmp(newValue, "***.***") == 0) { 
    freq_display.showString("888888");
  } else {
    double freqval = atof(newValue);
    if (freqval > 0) {
      freq_display.showNumber(freqval);
    } else {
      freq_display.clear();
    }
  }
}

DcsBios::StringBuffer<2> uhfChanDispBuffer(UHF_CHAN_DISP_DISPLAY_ADDRESS, onUhfChanDispChange);
DcsBios::StringBuffer<7> uhfFreqDispBuffer(UHF_FREQ_DISP_DISPLAY_ADDRESS, onUhfFreqDispChange);

const byte uhfFuncKnbPins[4] = {A0, A3, A2, A4};
DcsBios::SwitchMultiPos uhfFuncKnb("UHF_FUNC_KNB", uhfFuncKnbPins, 4);
DcsBios::Switch3Pos uhfModeKnb("UHF_MODE_KNB", 7, 8);
DcsBios::Switch2Pos uhfSquelchSw("UHF_SQUELCH_SW", 9);
DcsBios::Switch2Pos uhfStatusBtn("UHF_STATUS_BTN", 12);
DcsBios::Switch2Pos uhfTestBtn("UHF_TEST_BTN", 13);
DcsBios::Switch2Pos uhfToneBtn("UHF_TONE_BTN", 11);
DcsBios::Potentiometer uhfVolKnb("UHF_VOL_KNB", A1);

void setup()
{
  displays_init();
  DcsBios::setup();
}

void loop()
{
  DcsBios::loop();
}