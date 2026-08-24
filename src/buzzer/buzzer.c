#include "buzzer.h"
#include "hardware.h"

static uint slice_num;

void buzzer_init(void){
    gpio_set_function(buzzer_pin, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(buzzer_pin);
    
    pwm_config config = pwm_get_default_config();
    float div = (float)clock_get_hz(clk_sys) / (1000 * 4096); // Calculate the divider for 1 kHz frequency with 12-bit resolution
    pwm_config_set_clkdiv(&config, div);
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(buzzer_pin, 0); // Start with the buzzer off
}

void buzzer_on(void){
    pwm_set_gpio_level(buzzer_pin, 2048); // Set to half of the max level (12-bit resolution)
}

void buzzer_off(void){
    pwm_set_gpio_level(buzzer_pin, 0); // Set to the min level (12-bit resolution)
}

void buzzer_beep(uint32_t duration_ms){
    buzzer_on();
    sleep_ms(duration_ms);
    buzzer_off();
    sleep_ms(50); // Short delay to avoid overlapping beeps
}

void buzzer_play_tone(uint32_t frequency, uint32_t duration_ms){
    if (frequency == 0) {
        buzzer_off();
        sleep_ms(duration_ms);
        return;
    }

    uint32_t level = 2048; // Half of the max level (12-bit resolution)
    float div = (float)clock_get_hz(clk_sys) / (frequency * 4096); // Calculate the divider for the desired frequency
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, div);
    pwm_init(slice_num, &config, true);
    
    pwm_set_gpio_level(buzzer_pin, level); // Set to half of the max level (12-bit resolution)
    sleep_ms(duration_ms);
    buzzer_off();
}