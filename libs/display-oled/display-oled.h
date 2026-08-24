#ifndef DISPLAY_OLED_H
#define DISPLAY_OLED_H

#include "pico/stdlib.h"

// Funções simplificadas para o usuário
void display_init(void);
void clean_display(void);
void write_text_display(const char *text, int x, int y);
void update_display(void);

#endif