#include <Wire.h>
// on Arduino: pin A4 (SDA), A5 (SCL), SSD1311 PMOLED Controller
// on Teensy: pin 18 (SDA), 19 (SCL), SSD1311 PMOLED Controller
#define OLED_Address      0x3C
#define OLED_Command_Mode 0x80
#define OLED_Data_Mode    0x40
  
void sendData(unsigned char data)
{
  Wire.beginTransmission(OLED_Address);
  Wire.write(OLED_Data_Mode);  
  Wire.write(data);
  Wire.endTransmission();
}
 
void sendCommand(unsigned char command)
{
  Wire.beginTransmission(OLED_Address);
  Wire.write(OLED_Command_Mode);
  Wire.write(command);
  Wire.endTransmission();
}
 
void sendString(const char *OLED_string)
{
  unsigned char i = 0;
  while (OLED_string[i])
  {
    sendData(OLED_string[i]);  // send characters in string to OLED, 1 by 1
    i++;
  }
}

void sendLine1(const char* string) {  
  sendCommand(0x80);
  sendString(string);
}

void sendLine2(const char* string) {
  sendCommand(0xC0);  
  sendString(string);
}

void sendLines(const char* line1, const char* line2) {
  // Clear display
  sendCommand(0x01);
  sendLine1(line1);
  sendLine2(line2);
}
 
void SSD1311_init()
{  
  Wire.begin();
  sendCommand(0x2A);     // Function Set, set RE=1
  sendCommand(0x08);     // Extended Function Set, 5-dot, disable cursor-invert, 2-line mode
  sendCommand(0x72);     // Function Selection B
  sendData(0x03);        // Set ROM [ROM A 0x03, ROM B 0x07, ROM C 0x0B]
  sendCommand(0x79);     // set SD=1, enable OLED Command Set
  sendCommand(0x81);     // Set Contrast Control [brightness]
  sendCommand(0xFF);     // default is 0x7F, 0xFF is max
  sendCommand(0x78);     // set SD=0, disable OLED Command Set
  sendCommand(0x28);     // Function Set, set RE=0
  sendCommand(0x01);     // Clear Display and set DDRAM location 00h
  sendCommand(0x0C);     // turn display ON, cursor OFF, blink OFF
}