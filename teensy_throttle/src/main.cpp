#include <ResponsiveAnalogRead.h>

#define JOYSTICK_SIZE 64

ResponsiveAnalogRead throttle_pot(16, true);
ResponsiveAnalogRead range_pot(17, true);
ResponsiveAnalogRead radar_pot(18, true);

int cur_throttle_pos = 0;

void setup() {
  
  // Buttons on inputs 0-14 - skip 13 - LED pin.
  for (int i = 1; i < 16; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  pinMode(16, INPUT);
  pinMode(17, INPUT);
  pinMode(18, INPUT);

  throttle_pot.setActivityThreshold(10);
  range_pot.setActivityThreshold(10);
  radar_pot.setActivityThreshold(10);
}

// Main loop
void loop() {

  // Off by one, we use inputs 1 and up to 15.
  for (int i = 1; i < 16; i++) {
    
    Joystick.button(i, 1 - digitalRead(i));
    Joystick.button(i+15, digitalRead(i));
  }

  throttle_pot.update();
  range_pot.update();
  radar_pot.update();

  // Analog/slider inputs. Does not use buttons.
  Joystick.X(range_pot.getValue() * 64);
  Joystick.Y(radar_pot.getValue() * 64);
 
  // Throttle
  // Raw value
  int t_raw = throttle_pot.getValue();   
  // pot moves from 451 to 645 on a 1024 scale, so 194 is travel - scale up by 1024/194, considering offset
  int translated = (t_raw  - 451) * 5.28;
  // Then we scale actual position from 451 to 1024.
  if (t_raw < 433) {
    // Always set throttle to zero when cutoff    
    if (cur_throttle_pos > 433) {
      // Cut off if we are moving down past 30
      Joystick.button(127, 1);
    } else {
      Joystick.button(127, 0);
    }
  } else {
    if (cur_throttle_pos < 433) {
      // To idle if we are moving up past 30
      Joystick.button(128, 1);
    } else {
      Joystick.button(128, 0);
    }
    // Only move throttle if not in cutoff   
  }
  // remember where we are - and don't go neg
  Joystick.Zrotate((1024 - min(1024, translated)) * 64);
  cur_throttle_pos = t_raw;
}
