#include <stdio.h>
#include "pico/stdlib.h"
#include "libs/bitdoglab.h"
#include "libs/hardware.h"


void buttons_init() {

    // Botões
    gpio_init(button_A);
    gpio_set_dir(button_A, GPIO_IN);
    gpio_pull_up(button_A);

    gpio_init(button_B);
    gpio_set_dir(button_B, GPIO_IN);
    gpio_pull_up(button_B);

    gpio_init(button_C);
    gpio_set_dir(button_C, GPIO_IN);
    gpio_pull_up(button_C);
}