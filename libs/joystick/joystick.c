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

uint16_t joystick_read_x(void){
    adc_select_input(0); // Select ADC0 for X-axis
    uint16_t x_value = adc_read(); // Read the X-axis value
    return x_value; // Return the X-axis value
}

uint16_t joystick_read_y(void){
    adc_select_input(1); // Select ADC1 for Y-axis
    uint16_t y_value = adc_read(); // Read the Y-axis value
    return y_value; // Return the Y-axis value
}

bool joystick_button_pressed(void){
    return !gpio_get(joystick_sw_pin); // Return true if the button is pressed (active low)
}

#define ADC_CENTER 2048
#define DEADZONE   300 // Tolerância do centro

typedef enum {
    JOY_CENTER = 0,
    JOY_UP,
    JOY_DOWN,
    JOY_LEFT,
    JOY_RIGHT
} JoystickDir;

JoystickDir joystick_get_direction(void) {
    uint16_t x = joystick_read_x();
    uint16_t y = joystick_read_y();

    if (x < (ADC_CENTER - DEADZONE)) return JOY_LEFT;
    if (x > (ADC_CENTER + DEADZONE)) return JOY_RIGHT;
    if (y < (ADC_CENTER - DEADZONE)) return JOY_DOWN; // Inverter se necessário
    if (y > (ADC_CENTER + DEADZONE)) return JOY_UP;

    return JOY_CENTER;
}