#include <stdio.h>
#include "pico/stdlib.h"
#include "libs/bitdoglab.h"
#include "libs/hardware.h"
#include "libs/neopixel.h"


// Matriz de LEDs
void led_matrix_init() {
    np_init();

}

void set_matrix_pixel(int r, int g, int b) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        np_set_pixel(i, r, g, b);
    }
    np_write();
}

