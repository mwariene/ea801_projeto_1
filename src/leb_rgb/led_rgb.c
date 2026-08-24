#include "pico/stdlib.h"   // Para funções gerais de GPIO
#include "hardware/pwm.h"  // Define GPIO_FUNC_PWM e funções do PWM
#include "bitdoglab.h"
#include "src/hardware.h"
#include "src/bitdoglab.h"
#include "hardware/pwm.h"

/* Configuração comum (sem PWM)
gpio_init(LED_RED);
gpio_set_dir(LED_RED, GPIO_OUT);
gpio_put(LED_RED, 0);

gpio_init(LED_BLUE);
gpio_set_dir(LED_BLUE, GPIO_OUT);
gpio_put(LED_BLUE, 0);

gpio_init(LED_GREEN);
gpio_set_dir(LED_GREEN, GPIO_OUT);
gpio_put(LED_GREEN, 0);
*/

void led_rgb_init() {

    // Configuração Pinos do LED RGB para a função PWM
    gpio_set_function(LED_RED, GPIO_FUNC_PWM);
    gpio_set_function(LED_GREEN, GPIO_FUNC_PWM);
    gpio_set_function(LED_BLUE, GPIO_FUNC_PWM);

    //  Quais slices de PWM controlam cada pino
    uint slice_red = pwm_gpio_to_slice_num(LED_RED);
    uint slice_green = pwm_gpio_to_slice_num(LED_GREEN);
    uint slice_blue = pwm_gpio_to_slice_num(LED_BLUE);

    // Definição da resolução (WRAP = 4095 dá 12 bits de resolução, igual ao Joystick)
    pwm_set_wrap(slice_red, 4095);
    pwm_set_wrap(slice_green, 4095);
    pwm_set_wrap(slice_blue, 4095);

    // Estado inicial
    pwm_set_gpio_level(LED_RED, 0);
    pwm_set_gpio_level(LED_GREEN, 0);
    pwm_set_gpio_level(LED_BLUE, 0);

    // Habilitação do sinal PWM
    pwm_set_enabled(slice_red, true);
    pwm_set_enabled(slice_green, true);
    pwm_set_enabled(slice_blue, true);

}