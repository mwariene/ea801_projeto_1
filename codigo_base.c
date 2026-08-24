#include <stdio.h>
#include "pico/stdlib.h"
#include "src/bitdoglab.h"
#include "src/hardware.h"
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
    gpio_init(button_A);
    gpio_set_dir(button_A, GPIO_IN);
    gpio_pull_up(button_A);

    gpio_init(button_B);
    gpio_set_dir(button_B, GPIO_IN);
    gpio_pull_up(button_B);

    gpio_init(button_C);
    gpio_set_dir(button_C, GPIO_IN);
    gpio_pull_up(button_C);

    // Joystick
    adc_init();
    adc_gpio_init(27); // GPIO 27 = joy_x (ADC1)
    adc_gpio_init(26); // GPIO 26 = joy_y (ADC0)

    gpio_init(joy_sw);
    gpio_set_dir(joy_sw, GPIO_IN);
    gpio_pull_up(joy_sw);

    // Display OLED
    i2c_init(OLED_I2C, 400 * 1000); // Frequência de 400 kHz
    gpio_set_function(OLED_SDA, GPIO_FUNC_I2C);
    gpio_set_function(OLED_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(OLED_SDA);
    gpio_pull_up(OLED_SCL);


}


int main() {
    hardware_init();

    uint16_t brilho = 0;

    /*while (true) {
        
        // Exemplo com Botões:
        // Pressionar Botão A deixa o LED Azul bem fraco (brilho = 200)
        if (gpio_get(button_A) == 0) {
            pwm_set_gpio_level(LED_BLUE, 400); 
        } 
        // Pressionar Botão B deixa o LED Azul no brilho máximo (brilho = 4095)
        else if (gpio_get(button_B) == 0) {
            pwm_set_gpio_level(LED_BLUE, 4095); 
        } else {
            pwm_set_gpio_level(LED_BLUE, 0);
        }
    }*/
    while (true) {
        if (gpio_get(joy_sw) == 0) {
            set_matrix_pixel(0,0,30);
        }
        else if (gpio_get(button_B)==0){
            set_matrix_pixel(15, 0, 15);
        }
        else if(gpio_get(button_A)==0){
            pwm_set_gpio_level(LED_GREEN,400);
        }
        else {
            set_matrix_pixel(9,8,1);
            pwm_set_gpio_level(LED_GREEN,0);

        }
        sleep_ms(20);
    }

        // 3. Desenha um ponto verde na coordenada X=2, Y=2 (centro da matriz 5x5)
        /*int centro = np_get_index(2, 2);
        np_set_pixel(centro, 0, 20, 0); 
        np_write();
        sleep_ms(1000);

        np_clear();
        np_write();
        sleep_ms(500);*/
    }
  