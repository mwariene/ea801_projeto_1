#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "joystick.h"

void joystick_init(void){
    stdio_init_all();
    adc_init();
    adc_gpio_init(joystick_x_pin); // ADC0 - X-axis
    adc_gpio_init(joystick_y_pin); // ADC1 - Y-axis

    gpio_init(joystick_sw_pin);
    gpio_set_dir(joystick_sw_pin, GPIO_IN); 
    gpio_pull_up(joystick_sw_pin); // Enable pull-up resistor for the switch
}

uint8_t joystick_read_x(void){
    adc_select_input(0); // Select ADC0 for X-axis
    return adc_read(); // Read and return the X-axis value
}

uint8_t joystick_read_y(void){
    adc_select_input(1); // Select ADC1 for Y-axis
    return adc_read(); // Read and return the Y-axis value
}

bool joystick_button_pressed(void){
    return !gpio_get(joystick_sw_pin); // Return true if the button is pressed (active low)
}