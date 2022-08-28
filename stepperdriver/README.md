# ESP32 stepper motor driver for DCS bios

This is an ESP32 based stepper motor driver for DCS BIOS, with PlatformIO setup. 

It was born out of a need to drive many steppers from as few controllers as possible, using the FastAccelStepper library:
https://github.com/gin66/FastAccelStepper

Using this, "regular" Arduinos can drive up to three motors. However, for an F-16 simulator pit, like mine, that means a lot of arduinos. So, this:

The ESP32 can drive as many as you can fit on it's physical pins, mainly due to power alone - a dual core CPU at much higher speeds, and interrupt capability.

This sketch has an accompanying PCB, and provides for 9 steppers, and an encoder for pre-start zero-adjustment, and a switch to toggle which stepper motor to adjust. 