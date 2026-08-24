#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "pico/stdlib.h"

#define joystick_x_pin 26 
#define joystick_y_pin 27
#define joystick_sw_pin 22

void joystick_init(void);
uint8_t joystick_read_x(void);
uint8_t joystick_read_y(void);
bool joystick_button_pressed(void);
#endif