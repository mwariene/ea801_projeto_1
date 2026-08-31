#include <stdio.h>
#include "pico/stdlib.h"
#include "libs/bitdoglab.h"
#include "libs/hardware.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"


// int main() {
//     hardware_init();

//     while (true) {
//         while (gpio_get(button_A) == 0) {
//             set_matrix_all(5, 0, 0, true, 300); // Vermelho piscando
//         }
        

//         set_matrix_all(0, 0, 0, false, 0); // Apaga só quando nenhum botão está pressionado


//         sleep_ms(50); // Debounce mais rápido
//     }
// }

#include <stdio.h>
#include "pico/stdlib.h"
#include "libs/bitdoglab.h"
#include "libs/hardware.h"
#include "libs/display-oled/display-oled.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"

void hardware_init() {
    stdio_init_all();
    led_rgb_init();
    led_matrix_init();
    buttons_init();
    display_init();
}

int main() {
    hardware_init();
    
    clean_display();
    draw_text_buffer("EA801", 40, 0);
    draw_text_buffer("Projeto 1", 35, 10);
    draw_text_buffer("BitDogLab", 30, 20);
    update_display();

    while (true) {
        if (gpio_get(button_A) == 0) {
            set_matrix_all(5, 0, 0, true, 300);
            clear_buffer();
            draw_text_buffer("EA801", 40, 0);
            draw_text_buffer("Botao A", 40, 30);
            update_display();
        } else if (gpio_get(button_B) == 0) {
            set_matrix_all(3, 3, 0, false, 300);
            clear_buffer();
            draw_text_buffer("EA801", 40, 0);
            draw_text_buffer("Botao B", 40, 30);
            update_display();
        } else if (gpio_get(button_C) == 0) {
            set_matrix_pixel(12, 0, 4, 0, true, 1000);
            clear_buffer();
            draw_text_buffer("EA801", 40, 0);
            draw_text_buffer("Botao C", 40, 30);
            update_display();
        } else {
            set_matrix_all(0, 0, 0, false, 0);
            clear_buffer();
            draw_text_buffer("EA801", 40, 0);
            draw_text_buffer("Aguardando...", 25, 30);
            update_display();
        }

        sleep_ms(50);
    }
}