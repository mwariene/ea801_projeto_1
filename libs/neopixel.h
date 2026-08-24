#ifndef NEOPIXEL_H
#define NEOPIXEL_H

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "bitdoglab.h"
#include "ws2812.pio.h" // Gerado automaticamente pelo CMake

#define NUM_PIXELS 25

static PIO np_pio = pio0;
static uint np_sm = 0;
static uint32_t pixels[NUM_PIXELS];

// Inicializa o PIO para a Matriz
static inline void np_init() {
    uint offset = pio_add_program(np_pio, &ws2812_program);
    ws2812_program_init(np_pio, np_sm, offset, NEOPIXEL_PIN, 800000, false);
}

// Define a cor de um LED (Índice de 0 a 24). Valores de R, G, B de 0 a 255.
static inline void np_set_pixel(uint index, uint8_t r, uint8_t g, uint8_t b) {
    if (index < NUM_PIXELS) {
        // Formato GRB de 24 bits
        pixels[index] = ((uint32_t)(g) << 16) | ((uint32_t)(r) << 8) | (uint32_t)(b);
    }
}

// Apaga o buffer de todos os LEDs
static inline void np_clear() {
    for (int i = 0; i < NUM_PIXELS; i++) {
        pixels[i] = 0;
    }
}

// Envia os dados do buffer para a matriz física
static inline void np_write() {
    for (int i = 0; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(np_pio, np_sm, pixels[i] << 8u);
    }
    sleep_us(300);
}

// Converte coordenadas x, y (0 a 4) para o índice linear (0 a 24)
static inline int np_get_index(int x, int y) {
    // Matriz em formato zig-zag da BitDogLab
    if (y % 2 == 0) {
        return y * 5 + x;
    } else {
        return y * 5 + (4 - x);
    }
}

#endif