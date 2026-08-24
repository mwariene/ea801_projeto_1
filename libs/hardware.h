#ifndef HARDWARE_H
#define HARDWARE_H

#include "pico/stdlib.h"

// Funções de Inicialização Geral
void hardware_init(void);

// Funções do LED RGB
void led_rgb_init(void);

// Funções da Matriz de LEDs
void led_matrix_init(void);
void set_matrix_pixel(int r, int g, int b);

#endif

