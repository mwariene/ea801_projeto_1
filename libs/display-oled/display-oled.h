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

#endif