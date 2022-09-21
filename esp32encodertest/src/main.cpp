#include <ESP32Encoder.h>
#include <Adafruit_SSD1306.h>
#include <Button2.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C  // 128x32
#define OLED_RESET     -1
#define I2C_SDA 23
#define I2C_SCK 22
#define I2C_HZ 400000
#define SWITCH_PIN 0

TwoWire I2Cone = TwoWire(0);
ESP32Encoder encoder;
Button2 select_button(SWITCH_PIN);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2Cone, OLED_RESET);

String data = "F-16";
unsigned int click_counter = 0;
unsigned int adjust_multiplier  = 8;
unsigned int selected_stepper = 0;

void updateDisplay(String data) {
  display.clearDisplay();
  display.dim(true);
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(data);
  display.display();
}

// called by switch callback
void selector_clickhandler(Button2 &button) {
  switch (button.getType()) {
    case single_click:
      if (adjust_multiplier == 8) { adjust_multiplier = 1; }
      if (adjust_multiplier == 1) { adjust_multiplier = 8; }
      break;
    case long_click:
      click_counter += 1;
      selected_stepper = click_counter % (4);
      break;
  }
}

void setup(){
  I2Cone.begin(I2C_SDA, I2C_SCK, (uint32_t) I2C_HZ);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
	updateDisplay("init");
	ESP32Encoder::useInternalWeakPullResistors=UP;
	encoder.attachFullQuad(2, 15);
	encoder.setCount(0);
	updateDisplay(String((int32_t)encoder.getCount()));
  select_button.begin(SWITCH_PIN);
  select_button.setLongClickTime(500);  //ms
  select_button.setLongClickHandler(selector_clickhandler);
  select_button.setClickHandler(selector_clickhandler);
}

void loop(){
	// Loop and read the count
  select_button.loop();
  updateDisplay(String(selected_stepper) + String(adjust_multiplier) + " " + String((int32_t)encoder.getCount()));
}