#define DCSBIOS_RS485_SLAVE 98
#define TXENABLE_PIN 2
#include <DcsBios.h>
#include "Adafruit_PWMServoDriver.h"
#include "f16c_data.h"

Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver();

DcsBios::Switch3Pos oxyEmergLvr("OXY_EMERG_LVR", 7, 6);
DcsBios::Switch2Pos oxyDiluterLvr("OXY_DILUTER_LVR", 5);
DcsBios::Switch3Pos oxySupplyLvr("OXY_SUPPLY_LVR", 3, 4);

void onOxygenPressureChange(unsigned int newValue) {
    servos.setPWM(0, 0, map(newValue, 0, 65535, 500, 100));
}

void onFlowIndicatorChange(unsigned int newValue) {
}

DcsBios::IntegerBuffer oxygenPressureBuffer(OXYGEN_PRESSURE_GAUGE_ADDRESS, OXYGEN_PRESSURE_GAUGE_MASK, OXYGEN_PRESSURE_GAUGE_SHIFTBY, onOxygenPressureChange);
DcsBios::IntegerBuffer flowIndicatorBuffer(FLOW_INDICATOR_GAUGE_ADDRESS, FLOW_INDICATOR_GAUGE_MASK, FLOW_INDICATOR_GAUGE_SHIFTBY, onFlowIndicatorChange);

void setup () {
    servos.begin();
    servos.setPWMFreq(50);    
    servos.setPWM(0, 0, 500);
    delay(2000);
    servos.setPWM(0, 0, 100);
    delay(2000);
    servos.setPWM(0, 0, 300);
    DcsBios::setup();
}

void loop() {
    DcsBios::loop();
}