#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "libs/bitdoglab.h"
#include "libs/hardware.h"

void joystick_init() {
    // Joystick
    adc_init();
    adc_gpio_init(27); // GPIO 27 = joy_x (ADC1)
    adc_gpio_init(26); // GPIO 26 = joy_y (ADC0)

    gpio_init(joy_sw);
    gpio_set_dir(joy_sw, GPIO_IN);
    gpio_pull_up(joy_sw);
}
