#include <stdio.h>
#include "pico/stdlib.h"
#include "libs/bitdoglab.h"
#include "libs/hardware.h"
#include "libs/display-oled/display-oled.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"

// Enum for menu states
typedef enum {
    CURSOR_DEFAULT,
    DISPLAY_DEFAULT,
    DISPLAY_START,
    DISPLAY_INFO,
    DISPLAY_BACK
} MenuState;

void hardware_init()
{
    stdio_init_all();
    led_rgb_init();
    led_matrix_init();
    buttons_init();
    display_init();
}

// int

int main() {
    hardware_init();

    bool code = true;
    float v_lim = 100; // Velocidade limite:  100 m/s
    MenuState cursor = CURSOR_DEFAULT;       // Seleciona opções no display
    MenuState display = DISPLAY_DEFAULT;

    while (code)
    {
        // Tela inicial
        if (gpio_get(button_A) == 0) {
            display = DISPLAY_START;
        }
        else if (gpio_get(button_B) == 0) {
            cursor = DISPLAY_INFO;
        }
        else if (gpio_get(button_C) == 0) {
            cursor = DISPLAY_BACK;
        }

        // Tela inicial

        switch (cursor) {

        case DISPLAY_START:
            clear_buffer();
            draw_text_buffer("START", 0, 50);
            update_display();
        
        case DISPLAY_INFO:
            clear_buffer();
            draw_text_buffer("INFOS", 0, 50);
            update_display();

        case DISPLAY_BACK:
            draw_text_buffer("TELA INICIAL", 0, 60);
            update_display();

        default:
            draw_text_buffer("TELA INICIAL", 0, 60);
            update_display();
        }
        sleep_ms(50);
    }
}