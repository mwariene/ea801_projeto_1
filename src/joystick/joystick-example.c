#include <stdio.h>
#include "pico/stdlib.h"
#include "joystick.h"

int main(){
    stdio_init_all();
    joystick_init();

    while(true){
        uint16_t x_value = joystick_read_x();
        uint16_t y_value = joystick_read_y();
        bool button_pressed = joystick_button_pressed();
        
        printf("Joystick X: %d, Y: %d, Button Pressed: %s\n", x_value, y_value, button_pressed ? "Yes" : "No");

        sleep_ms(500); // Delay for readability
    }
}