#include <stdio.h>
#include "pico/stdlib.h"
#include "libs/bitdoglab.h"
#include "libs/hardware.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"



void hardware_init() {
    stdio_init_all(); // Inicializa comunicação via USB

    // LEG RGB
    led_rgb_init();
    
    // Matriz de LEDs
    led_matrix_init();

    // Botões
    buttons_init();

    
    // Display OLED
    i2c_init(OLED_I2C, 400 * 1000); // Frequência de 400 kHz
    gpio_set_function(OLED_SDA, GPIO_FUNC_I2C);
    gpio_set_function(OLED_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(OLED_SDA);
    gpio_pull_up(OLED_SCL);


}


int main() {
    hardware_init();

    while (true) {
        while (gpio_get(button_A) == 0) {
            set_matrix_all(5, 0, 0, true, 300); // Pisca vermelho
        }
        while (gpio_get(button_B) == 0) {
            set_matrix_all(3, 3, 0, false, 300); // Mantém amarelo
        }

        set_matrix_all(0, 0, 0, false, 0); // Apaga a matriz quando nenhum botão está pressionado.
    }
}
  