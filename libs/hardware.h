#ifndef HARDWARE_H
#define HARDWARE_H

#include "pico/stdlib.h"

// Funções de Inicialização Geral
void hardware_init(void);

// Funções de inputs
void buttons_init(void);
void joystick_init(void);

// Funções do LED RGB
void led_rgb_init(void);

// Funções da Matriz de LEDs
void led_matrix_init(void);
void set_matrix_pixel(int i, int r, int g, int b, bool blink, int sleep_time);
void set_matrix_all(int r, int g, int b, bool blink, int sleep_time);


#endif

