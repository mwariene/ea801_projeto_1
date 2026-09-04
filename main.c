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
} State;

int main(){
    stdio_init_all();

    display_init();
    clean_display();
    joystick_init();

    State current_state = state_menu;

    State previous_state = -1;

    while (true) {
        if (current_state != previous_state) {
        clear_buffer();
        previous_state = current_state;
    }
        switch(current_state){
            case state_menu:
                clean_display();
                draw_text_buffer("Menu", 10,0);
                draw_text_buffer("Start", 5, 10);
                draw_text_buffer("Info",5,20);
                update_display();
                
                uint16_t joy_y = joystick_read_y();
                if (joy_y < 2000){
                    current_state = state_start;
                } else if (joy_y > 2100){
                    current_state = state_info;
                }
                else if (joy_y > 2005 || joy_y < 2098){
                    current_state = state_menu;
                }
                break;
            case state_start:
                clean_display();
                draw_text_buffer("Menu", 10,0);
                draw_text_buffer("Start", 5, 10);
                draw_text_buffer("Info",5,20);
                draw_circle_display(0,13,2);
                update_display();
                if (joystick_button_pressed()){
                    current_state = state_running;
                } else {
                    current_state = state_menu;
                }
                break;
            case state_info:
                clean_display();
                draw_text_buffer("Menu", 10,0);
                draw_text_buffer("Start", 5, 10);
                draw_text_buffer("Info",5,20);
                draw_circle_display(0,23,2);
                update_display();
                if (joystick_button_pressed()){
                    current_state = state_func;
                } else {
                    current_state = state_menu;
                }
                break;
            case state_running:
                clean_display();
                draw_text_buffer("Velocidade", 0, 0);
                draw_text_buffer("20km/h",50,10); // precisa adc a integracao com a velocidade
                draw_text_buffer("Velocidade Max", 0, 25);
                draw_text_buffer("100km/h",50,35);
                draw_text_buffer("Distancia", 0, 50);
                draw_text_buffer("100km/h",50,60);
                update_display();
                // adc lógica de velocidade
                if (joystick_button_pressed()){
                    current_state = state_stop;
                }
                break;
            case state_stop:
                clean_display();
                draw_text_buffer("Desligar", 10,0);
                draw_text_buffer("Sim", 5, 10);
                draw_text_buffer("Nao",5,20);
                update_display();
                
                uint16_t joy_y_stop = joystick_read_y();
                if (joy_y_stop < 2000){
                    current_state = state_stop_yes;
                } else if (joy_y_stop > 2100){
                    current_state = state_stop_no;
                }
                else if (joy_y > 2005 || joy_y < 2098){
                    current_state = state_stop;
                }
                break;
            case state_func:
                clean_display();
                draw_text_buffer("Voltar", 5, 10);
                draw_circle_display(0,13,2); 
                if (joystick_button_pressed()){
                    current_state = state_menu;
                }   
                break;
            case state_stop_yes:
                clean_display();
                draw_text_buffer("Desligar", 10,0);
                draw_text_buffer("Sim", 5, 10);
                draw_text_buffer("Nao",5,20);
                draw_circle_display(0,13,2);
                update_display(); 
                if (joystick_button_pressed()){
                    current_state = state_menu;
                }
                else {
                    current_state = state_stop;
                }
                break;
            case state_stop_no:
                clean_display();
                draw_text_buffer("Desligar", 10,0);
                draw_text_buffer("Sim", 5, 10);
                draw_text_buffer("Nao",5,20);
                draw_circle_display(0,23,2);
                update_display(); 
                if (joystick_button_pressed()){
                    current_state = state_running;
                }
                else {
                    current_state = state_stop;
                }
                break;
        }
        
        // Envia para o display físico
        sleep_ms(1);
    }
    
    return 0;
}