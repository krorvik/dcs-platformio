// Generates rolling PWM output values based on encoder inputs
// Decoded on other side

#include "ESPRotary.h"
#include "Button2.h"

#define ENC1_PIN_A 7
#define ENC1_PIN_B 8
#define ENC1_SW_PIN 9

#define ENC2_PIN_A 10
#define ENC2_PIN_B 11
#define ENC2_SW_PIN 12

#define PWM_OUTPUT1 3
#define SW_OUTPUT1 4

#define PWM_OUTPUT2 5
#define SW_OUTPUT2 6


ESPRotary enc1;
Button2 but1;

ESPRotary enc2;
Button2 but2;

void rotate1(ESPRotary& encoder) {
  analogWrite(PWM_OUTPUT1, encoder.getPosition() % 255);
}

void rotate2(ESPRotary& encoder) {
  analogWrite(PWM_OUTPUT2, encoder.getPosition() % 255);
}



void click1(Button2& button) { digitalWrite(SW_OUTPUT1, digitalRead(ENC1_SW_PIN)); }
void click2(Button2& button) { digitalWrite(SW_OUTPUT2, digitalRead(ENC2_SW_PIN));}

void setup() {  

  for(int i = 7; i<=12; i++) { pinMode(i, INPUT_PULLUP); }
  for(int i = 3; i<=6; i++) { pinMode(i, OUTPUT); }

  // init switch pins high, else first short click is taken as long click
  digitalWrite(ENC1_SW_PIN, HIGH);
  digitalWrite(ENC2_SW_PIN, HIGH);
  
  // Encoder setup
  enc1.begin(ENC1_PIN_A, ENC1_PIN_B);  
  enc1.setChangedHandler(rotate1);
  enc1.setStepsPerClick(4);
  enc1.resetPosition(INT16_MAX / 2, false);

  but1.begin(ENC1_SW_PIN);
  but1.setChangedHandler(click1);
  digitalWrite(SW_OUTPUT1, HIGH);

  enc2.begin(ENC2_PIN_A, ENC2_PIN_B);
  enc2.setChangedHandler(rotate2);
  enc2.setStepsPerClick(4);
  enc2.resetPosition(INT16_MAX / 2, false);

  but2.begin(ENC2_SW_PIN);
  but2.setChangedHandler(click2);
  
  // Init values
  analogWrite(PWM_OUTPUT1, 128);
  analogWrite(PWM_OUTPUT2, 128);

}

void loop() {
  enc1.loop();
  enc2.loop();
  but1.loop();
  but2.loop();
}

