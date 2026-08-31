#ifndef DISPLAY_H
#define DISPLAY_H

#include "pico/stdlib.h"

void init_display();
void display_text(const char *text, int x, int y);
void clear_display();
void write_pixel(int x, int y, bool on);;
void display_update();

#endif