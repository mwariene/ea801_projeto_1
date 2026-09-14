#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "display-oled.h"
#include "libs/bitdoglab.h"
#include "hardware.h"
#include "joystick.h"

typedef enum {
    state_menu, // 0
    state_start, // 1
    state_info, // 2
    state_running, // 3
    state_stop, // 4
    state_func, // 5
    state_stop_yes, // 6
    state_stop_no, // 7
    state_change_velocity, // 8
    state_reverse // 9
} State;

static void update_distance(uint8_t velocity_kmh, bool reverse,
                            int32_t *distance_m, uint32_t *time_remainder_ms,
                            uint32_t *last_ms) {
    uint32_t current_ms = to_ms_since_boot(get_absolute_time());
    uint32_t elapsed_ms = current_ms - *last_ms;
    uint64_t distance_numerator = (uint64_t)velocity_kmh * elapsed_ms
                                + *time_remainder_ms;

    int32_t distance_delta = distance_numerator / 3600;
    if (reverse) {
        *distance_m -= distance_delta;
        if (*distance_m < 0) {
            *distance_m = 0;
        }
    } else {
        *distance_m += distance_delta;
    }
    *time_remainder_ms = distance_numerator % 3600;
    *last_ms = current_ms;
}

int main(){
    stdio_init_all();
    buttons_init();
    led_matrix_init();
    led_rgb_init();
    display_init();
    clean_display();
    joystick_init();

    State current_state = state_menu;
    uint8_t menu_position = 0; // 0 = Start, 1 = Info

    State previous_state = -1;
    uint8_t previous_menu_position = 255;
    uint8_t stop_position = 0; // 0 = Sim, 1 = Nao
    State previous_stop_state = -1;
    uint8_t previous_stop_position = 255;
    bool button_released = true;
    
    uint8_t count_A = 0;
    uint8_t velocity_int = 0;
    uint8_t max_velocity = 100;
    int32_t distance_m = 0;
    uint32_t time_remainder_ms = 0;
    uint32_t last_distance_ms = to_ms_since_boot(get_absolute_time());
    uint32_t last_display_ms = 0;
    uint32_t reverse_zero_since_ms = 0;
    bool above_max_velocity = false;
    bool reverse_dir = false;
    char velocity[4];
    char distance[12];
    snprintf(velocity, sizeof(velocity), "%u", velocity_int);
    snprintf(distance, sizeof(distance), "%ld m", (long)distance_m);

    while (true) {
        bool button_pressed = joystick_button_pressed();
        bool button_clicked = button_pressed && button_released;
        button_released = !button_pressed;

        switch(current_state){
            case state_menu:
                uint16_t joy_y = joystick_read_y();
                if (joy_y < 1800) {
                    menu_position = 0;
                } else if (joy_y > 2300) {
                    menu_position = 1;
                }

                if (current_state != previous_state ||
                    menu_position != previous_menu_position) {
                    clear_buffer();
                    draw_text_buffer("Menu", 10,0);
                    draw_text_buffer("Start", 5, 10);
                    draw_text_buffer("Info",5,20);
                    draw_circle_display(0, menu_position == 0 ? 13 : 23, 2);
                    update_display();
                    previous_state = current_state;
                    previous_menu_position = menu_position;
                }

                if (button_clicked) {
                    current_state = menu_position == 0 ? state_running : state_func;
                    sleep_ms(20);
                }
                break;

            case state_start:
                clear_buffer();
                draw_text_buffer("Menu", 10,0);
                draw_text_buffer("Start", 5, 10);
                draw_text_buffer("Info",5,20);
                draw_circle_display(0,13,2);
                update_display();
                if (button_clicked){
                    current_state = state_running;
                    sleep_ms(20);
                } else {
                    current_state = state_menu;
                }
                break;

            case state_info:
                clear_buffer();
                draw_text_buffer("Menu", 10,0);
                draw_text_buffer("Start", 5, 10);
                draw_text_buffer("Info",5,20);
                draw_circle_display(0,23,2);
                update_display();
                if (button_clicked){
                    current_state = state_func;
                    sleep_ms(20);
                } else {
                    current_state = state_menu;
                }
                break;

            case state_running:
                {
                    update_distance(velocity_int, reverse_dir, &distance_m,
                                    &time_remainder_ms, &last_distance_ms);
                    snprintf(distance, sizeof(distance), "%ld m",
                             (long)distance_m);
                    uint32_t current_ms = to_ms_since_boot(get_absolute_time());
                    if (current_state != previous_state ||
                        current_ms - last_display_ms >= 500) {
                        clear_buffer();
                        draw_text_buffer("Velocidade", 0, 0);
                        draw_text_buffer("   km/h",50,10); // precisa adc a integracao com a velocidade
                        draw_text_buffer("Velocidade Max", 0, 25);
                        draw_text_buffer("100km/h",50,35);
                        draw_text_buffer("Distancia", 0, 45);
                        draw_text_buffer(distance,50,55);
                        update_display();
                        set_led_rgb(200,200,0);
                        last_display_ms = current_ms;
                        previous_state = current_state;
                    }
                }
                // adc lógica de velocidade
                if (button_clicked){
                    current_state = state_stop;
                    sleep_ms(20);
                }
                if (gpio_get(button_A) == 0){
                   current_state = state_change_velocity;
                   sleep_ms(20);
                }
                else if (gpio_get(button_B)==0) {
                    clear_buffer();
                    draw_text_buffer("50",50,10);
                    update_display();
                    set_matrix_all(10,0,0,false,300);
                }
                break;

            case state_stop:
                uint16_t joy_y_stop = joystick_read_y();
                if (joy_y_stop < 2000){
                    stop_position = 0;
                } else if (joy_y_stop > 2100){
                    stop_position = 1;
                }

                if (current_state != previous_stop_state ||
                    stop_position != previous_stop_position) {
                    clear_buffer();
                    draw_text_buffer("Desligar", 10,0);
                    draw_text_buffer("Sim", 5, 10);
                    draw_text_buffer("Nao",5,20);
                    draw_circle_display(0, stop_position == 0 ? 13 : 23, 2);
                    update_display();
                    previous_stop_state = current_state;
                    previous_stop_position = stop_position;
                }

                if (button_clicked) {
                    current_state = stop_position == 0 ? state_menu : state_running;
                    sleep_ms(20);
                }
                break;

            case state_func:
                clear_buffer();
                draw_text_buffer("Voltar", 5, 10);
                draw_text_buffer("Infos", 0, 30);
                draw_text_buffer("A: acelera", 0, 50);
                draw_text_buffer("B: desacelera", 0, 60);
                draw_text_buffer("C: muda direcao", 0, 70);
                draw_text_buffer("Mariana", 0, 110);
                draw_text_buffer("Marina", 0, 120);
                draw_circle_display(0,13,2); 
                update_display();
                if (button_clicked){
                    current_state = state_menu;
                    sleep_ms(20);
                }   
                break;

            case state_stop_yes:
                clear_buffer();
                draw_text_buffer("Desligar", 10,0);
                draw_text_buffer("Sim", 5, 10);
                draw_text_buffer("Nao",5,20);
                draw_circle_display(0,13,2);
                update_display(); 
                if (button_clicked){
                    current_state = state_menu;
                    sleep_ms(20);
                }
                else {
                    current_state = state_stop;
                }
                break;

            case state_stop_no:
                clear_buffer();
                draw_text_buffer("Desligar", 10,0);
                draw_text_buffer("Sim", 5, 10);
                draw_text_buffer("Nao",5,20);
                draw_circle_display(0,23,2);
                update_display(); 
                if (button_clicked){
                    current_state = state_running;
                    sleep_ms(20);
                }
                else {
                    current_state = state_stop;
                }
                break;

            case state_change_velocity:
                update_distance(velocity_int, reverse_dir, &distance_m,
                                &time_remainder_ms, &last_distance_ms);
                snprintf(distance, sizeof(distance), "%ld m",
                         (long)distance_m);

                if (reverse_dir && velocity_int == 0) {
                    uint32_t current_ms = to_ms_since_boot(get_absolute_time());
                    bool action = button_clicked || gpio_get(button_A) == 0 ||
                                  gpio_get(button_B) == 0 || gpio_get(button_C) == 0;

                    if (action) {
                        reverse_zero_since_ms = current_ms;
                    } else if (reverse_zero_since_ms == 0) {
                        reverse_zero_since_ms = current_ms;
                    } else if (current_ms - reverse_zero_since_ms >= 5000) { // Aguarda 5 segundos
                        reverse_dir = false;
                        clear_buffer();
                        draw_text_buffer("desligando", 0, 40);
                        sleep_ms(50);
                        draw_text_buffer("desligando.",0 , 40);
                        sleep_ms(50);
                        draw_text_buffer("desligando..", 0, 40);
                        sleep_ms(50);
                        draw_text_buffer("desligando...", 0, 40);
                        update_display();
                        sleep_ms(2000);
                        clear_buffer();
                        update_display();
                        current_state = state_start;
                        reverse_zero_since_ms = 0;
                    }
                } else {
                    reverse_zero_since_ms = 0;
                }

                if (velocity_int >= max_velocity && !above_max_velocity){ // Atinge o limite
                    set_matrix_all(5,0,0,false,0);
                    draw_text_buffer("Limite", 0, 80);
                    draw_text_buffer("Atingido", 0, 90);
                    draw_text_buffer("A: continuar", 0, 110);
                    draw_text_buffer("B: parar", 0, 120);
                    update_display();

                    if (gpio_get(button_A)==0){
                        set_matrix_all(2,0,0,false,0);
                        above_max_velocity = true;
                    }
                    else if (gpio_get(button_B)==0){
                        set_matrix_all(0,0,0,false,0);
                        velocity_int = 0;
                        above_max_velocity = false;
                        current_state = state_start;
                    }
                    
                }
                else if (velocity_int < 0) {
                    velocity_int = 0;
                    set_matrix_all(1,1,0,false,0);
                    snprintf(velocity, sizeof(velocity), "%u", velocity_int);

                    clear_buffer();
                    draw_text_buffer("Velocidade", 0, 0);
                    draw_text_buffer("   km/h",50,10);
                    draw_text_buffer("0", 47, 10);
                    draw_text_buffer("Velocidade Max", 0, 25);
                    draw_text_buffer("100km/h",50,35);
                    draw_text_buffer("Distancia", 0, 45);
                    draw_text_buffer(distance,50,55);
                    update_display();
                    sleep_ms(20);
                }
                else if (reverse_dir || (velocity_int == 0 && gpio_get(button_C)==0)) {
                    reverse_dir = true;
                    set_matrix_all(0,0,0,false,0);
                    set_led_rgb(0,0,200);

                    if (gpio_get(button_A) == 0) {
                        velocity_int += 10;
                    }
                    else if (gpio_get(button_B) == 0) {
                        if (velocity_int >= 10) {
                            velocity_int -= 10;
                            set_led_rgb(200,0,0);
                        }
                        if (velocity_int == 0) {
                            reverse_dir = false;
                            set_led_rgb(0,0,0);

                        }
                    }

                    snprintf(velocity, sizeof(velocity), "%u", velocity_int);
                    clear_buffer();
                    draw_text_buffer("Velocidade", 0, 0);
                    draw_text_buffer("   km/h",50,10);
                    draw_text_buffer(velocity, 47, 10);
                    draw_text_buffer("Velocidade Max", 0, 25);
                    draw_text_buffer("100km/h",50,35);
                    draw_text_buffer("Distancia", 0, 45);
                    draw_text_buffer(distance,50,55);
                    draw_text_buffer("REVERSE",20,65);
                    update_display();
                    sleep_ms(20);
                }
                else {
                    set_matrix_all(0,0,0,false,0);
                    if (gpio_get(button_A) == 0) {
                        velocity_int += 10;
                        snprintf(velocity, sizeof(velocity), "%u", velocity_int);

                        clear_buffer();
                        draw_text_buffer("Velocidade", 0, 0);
                        draw_text_buffer("   km/h",50,10);
                        draw_text_buffer(velocity, 47, 10);
                        draw_text_buffer("Velocidade Max", 0, 25);
                        draw_text_buffer("100km/h",50,35);
                        draw_text_buffer("Distancia", 0, 45);
                        draw_text_buffer(distance,50,55);
                        set_led_rgb(0,200,0);
                        update_display();
                        sleep_ms(20);
                    }

                    else if (gpio_get(button_B)==0){
                        if (velocity_int >= 10) {
                            velocity_int -= 10;
                            set_led_rgb(200,0,0);
                        }
                        else {
                            velocity_int = 0;
                            above_max_velocity = false;
                        }
                        snprintf(velocity, sizeof(velocity), "%u", velocity_int);

                        if (!reverse_dir) { // Modo normal
                            clear_buffer();
                            draw_text_buffer("Velocidade", 0, 0);
                            draw_text_buffer("   km/h",50,10);
                            draw_text_buffer(velocity, 47, 10);
                            draw_text_buffer("Velocidade Max", 0, 25);
                            draw_text_buffer("100km/h",50,35);
                            draw_text_buffer("Distancia", 0, 45);
                            draw_text_buffer(distance,50,55);
                            update_display();
                            sleep_ms(20);
                        }
                        else {   // Modo ré
                            clear_buffer();
                            draw_text_buffer("Velocidade", 0, 0);
                            draw_text_buffer("   km/h",50,10);
                            draw_text_buffer(velocity, 47, 10);
                            draw_text_buffer("Velocidade Max", 0, 25);
                            draw_text_buffer("100km/h",50,35);
                            draw_text_buffer("Distancia", 0, 45);
                            draw_text_buffer(distance,50,55);
                            draw_text_buffer("REVERSE",20,80);
                            update_display();
                            sleep_ms(20);
                        }
                        
                    }
                }

                uint32_t current_change_ms = to_ms_since_boot(get_absolute_time());
                if (current_change_ms - last_display_ms >= 500) {
                    clear_buffer();
                    draw_text_buffer("Velocidade", 0, 0);
                    draw_text_buffer("   km/h",50,10);
                    draw_text_buffer(velocity, 47, 10);
                    draw_text_buffer("Velocidade Max", 0, 25);
                    draw_text_buffer("100km/h",50,35);
                    draw_text_buffer("Distancia", 0, 45);
                    draw_text_buffer(distance,50,55);
                    if (reverse_dir) {
                        draw_text_buffer("REVERSE",20,65);
                    }
                    else{
                        draw_text_buffer("frente",20,65);
                    }
                    update_display();
                    last_display_ms = current_change_ms;
                }

                if (button_clicked) {
                    current_state = state_running;
                    sleep_ms(20);
                }
                break;
        }
        
        // Envia para o display físico
        sleep_ms(1);
    }
    
    return 0;
}