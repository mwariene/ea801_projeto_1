#ifndef DISPLAY_H
#define DISPLAY_H

#include "pico/stdlib.h"

// Funções simplificadas para o usuário
void display_init(void);
void clean_display(void);
void write_text_display(const char *text, int x, int y);
void clear_buffer(void);
void draw_text_buffer(const char *text, int x, int y);
void update_display(void);
void draw_pixel_display(int x, int y, bool set);
void draw_line_display(int x1, int y1, int x2, int y2);
void draw_rect_display(int x1, int y1, int x2, int y2);
void draw_circle_display(int center_x, int center_y, int radius);

#endif