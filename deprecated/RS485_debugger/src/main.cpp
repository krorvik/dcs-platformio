#define DCSBIOS_IRQ_SERIAL
#include <DcsBios.h>
#include "f16c_data.h"
#include "op162i.h"

char *DESC = "UHF CHAN";

void onChange(char* newValue) { sendLines(DESC, newValue); }

//DcsBios::StringBuffer<7> uhfFreqDispBuffer(UHF_FREQ_DISP_DISPLAY_ADDRESS, onChange);
DcsBios::StringBuffer<2> uhfChanDispBuffer(UHF_CHAN_DISP_DISPLAY_ADDRESS, onChange);


void setup() {
    delay(100);
    SSD1311_init();
    sendLines("NOT", "RUNNING");
    DcsBios::setup();
}

void loop () {
    DcsBios::loop();
}