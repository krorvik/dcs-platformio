#define DCSBIOS_RS485_SLAVE 98
#define TXENABLE_PIN 2
#include <DcsBios.h>
#include "f16c_data.h"
#include "FastAccelStepper.h"
#include "Adafruit_SSD1306.h"

#define DISPLAY_WIDTH 128 // OLED display width, in pixels
#define DISPLAY_HEIGHT 32 // OLED display height, in pixels
#define DISPLAY_ADDRESS 0x3C

#define SPEED 8000 
#define ACCELERATION 8000
#define STEPS_MAX  4300

FastAccelStepperEngine stepper_engine = FastAccelStepperEngine();
FastAccelStepper *psi_stepper = NULL;
Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT);

DcsBios::Switch3Pos oxyEmergLvr("OXY_EMERG_LVR", 3,4);
DcsBios::Switch2Pos oxyDiluterLvr("OXY_DILUTER_LVR", 5);
DcsBios::Switch3Pos oxySupplyLvr("OXY_SUPPLY_LVR", 10,12);

void stepper_init() {
  stepper_engine.init();
  psi_stepper = stepper_engine.stepperConnectToPin(9);
  psi_stepper->setDirectionPin(11);
  psi_stepper->setSpeedInHz(SPEED);
  psi_stepper->setAcceleration(ACCELERATION);
  psi_stepper->setCurrentPosition(0);
}

void sweep() {
    psi_stepper->moveTo(-5760);
    while(psi_stepper->isRunning()) {}
    psi_stepper->setCurrentPosition(0);
    psi_stepper->moveTo(STEPS_MAX);
    while(psi_stepper->isRunning()) {}
    delay(1000);
    psi_stepper->moveTo(0);
    while(psi_stepper->isRunning()) {}
}

void drawFlow(unsigned int value) {
    display.clearDisplay();
    display.dim(true);
    display.fillRect(0, 0, map(value, 0, 65535, 0, 100), 32, SSD1306_WHITE);
    display.display();
}

void onOxygenPressureChange(unsigned int newValue) { psi_stepper->moveTo(map(newValue, 0, 65535, 0, STEPS_MAX)); }
void onFlowIndicatorChange(unsigned int newValue) { drawFlow(newValue); }

DcsBios::IntegerBuffer oxygenPressureBuffer(OXYGEN_PRESSURE_GAUGE_ADDRESS, OXYGEN_PRESSURE_GAUGE_MASK, OXYGEN_PRESSURE_GAUGE_SHIFTBY, onOxygenPressureChange);
DcsBios::IntegerBuffer flowIndicatorBuffer(FLOW_INDICATOR_GAUGE_ADDRESS, FLOW_INDICATOR_GAUGE_MASK, FLOW_INDICATOR_GAUGE_SHIFTBY, onFlowIndicatorChange);

void setup () {   
    stepper_init();
    display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS);
    delay(500);
    display.clearDisplay();    
    display.display();
    drawFlow(32768);
    sweep();
    drawFlow(0);
    DcsBios::setup();
}

void loop() {
    DcsBios::loop();
}