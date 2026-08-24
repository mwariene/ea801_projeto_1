#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "libs/bitdoglab.h"
#include "libs/hardware.h"
#include "libs/neopixel.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"


// Matriz de LEDs
void led_matrix_init() {
    np_init();
    np_clear();
    np_write();
}

void set_matrix_pixel(int i, int r, int g, int b, bool blink, int sleep_time) {
    if (i>=0 & i<=25){
        np_set_pixel(i, r, g, b);
    }
}


void set_matrix_all(int r, int g, int b, bool blink, int sleep_time){
    
    for (int i = 0; i < NUM_PIXELS; i++) {
        np_set_pixel(i, r, g, b);
    }
    np_write();

    if (blink) {
        sleep_ms(sleep_time);
        np_clear();
        np_write();
        sleep_ms(sleep_time);
    }
}

