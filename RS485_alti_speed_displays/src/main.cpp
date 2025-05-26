#define DCSBIOS_RS485_SLAVE 42
#define TXENABLE_PIN 2
#include "DcsBios.h"
#include <Adafruit_SSD1306.h>
#include "f16c_data.h"
#include <Wire.h>

#define PNEU_PIN 9

#define MUX_ADDR 0x70
#define SCREEN_WIDTH_BIG 128 // OLED display width, in pixels
#define SCREEN_WIDTH_SMALL 64 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C 
#define OLED_RESET     -1

Adafruit_SSD1306 display_1(SCREEN_WIDTH_SMALL, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display_2(SCREEN_WIDTH_BIG, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display_3(SCREEN_WIDTH_SMALL, SCREEN_HEIGHT, &Wire, OLED_RESET);

char initchars[4] = { '-', 'O', '-', '\0'};

void selectBus(uint8_t bus) {
  Wire.beginTransmission(MUX_ADDR);
  Wire.write(1 << bus);
  Wire.endTransmission();
}

void initDisplays() {
  Wire.begin();
  selectBus(1);
  display_1.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  selectBus(2);
  display_2.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  selectBus(3);
  display_3.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
}

void update_d1() {  
  selectBus(1);
  display_1.clearDisplay();
  display_1.dim(true);
  display_1.setTextSize(4);
  display_1.setTextColor(WHITE);
  display_1.setCursor(25, 0);
  display_1.println(initchars);
  display_1.display();
}

void update_d2() {  
  selectBus(2);
  display_2.clearDisplay();
  display_2.dim(true);
  display_2.setTextSize(4);
  display_2.setTextColor(WHITE);
  display_2.setCursor(25, 0);
  display_2.println(initchars);
  display_2.display();
}

void update_d3() {  
  selectBus(3);
  display_3.clearDisplay();
  display_3.dim(true);
  display_3.setTextSize(4);
  display_3.setTextColor(WHITE);
  display_3.setCursor(25, 0);
  display_3.println(initchars);
  display_3.display();
}

void onAltPneuFlagChange(unsigned int newValue) {
    analogWrite(PNEU_PIN, map(newValue, 0, 65535, 0, 255));
}

DcsBios::IntegerBuffer altPneuFlagBuffer(ALT_PNEU_FLAG_GAUGE_ADDRESS, ALT_PNEU_FLAG_GAUGE_MASK, ALT_PNEU_FLAG_GAUGE_SHIFTBY, onAltPneuFlagChange);

void setup() {
  pinMode(PNEU_PIN, OUTPUT);
  analogWrite(PNEU_PIN, 0);
  delay(1000);
  analogWrite(PNEU_PIN, 1);
  DcsBios::setup();  
  initDisplays();
  update_d1();
  update_d2();
  update_d3();
  
}

void loop() {
  DcsBios::loop();
  update_d1();
  update_d2();
  update_d3();
}
