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

    draw_text_buffer("Menu", 10,0);
    draw_text_buffer("Start", 5, 10);
    draw_text_buffer("Info",5,20);

    uint16_t joy_y = joystick_read_y();
    uint16_t joy_x = joystick_read_x();

    while (true) {
        clearbuffer();
        State = case 0;
        switch(State){
            case 0:
                draw_text_buffer("Menu", 10,0);
                draw_text_buffer("Start", 5, 10);
                draw_text_buffer("Info",5,20);
                update_display();
                if (joy_y < 2048){
                    State = state_start;
                else if (joy_y > 2048){
                    State = state_info;
                    }
                }
                break;
            case 1:
                clean_display();
                draw_text_buffer("Menu", 10,0);
                draw_text_buffer("Start", 5, 10);
                draw_text_buffer("Info",5,20);
                draw_circle_display(0,13,2);
                update_display();
                if (joystick_button_pressed()){
                    State = state_running;
                }
                else (){
                    State = case 0;
                }
                break;
            case 2:
                clean_display();
                draw_text_buffer("Menu", 10,0);
                draw_text_buffer("Start", 5, 10);
                draw_text_buffer("Info",5,20);
                draw_circle_display(0,23,2);
                update_display();
                if (joystick_button_pressed()){
                    State = state_func;
                }
                else (){
                    State = case 0;
                }
                break;
            case 3:
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
                    State = state_stop;
                }
                break;
            case 4:
                clean_display();
                draw_text_buffer("Desligar", 10,0);
                draw_text_buffer("Sim", 5, 10);
                draw_text_buffer("Nao",5,20);
                update_display();
                if (joy_y < 2048){
                    State = state_stop_yes;
                elseif (joy_y > 2048){
                    State = state_stop_no;
                    }
                }
                break;
            case 5:
                clean_display();
                draw_text_buffer("Voltar", 5, 10);
                draw_circle_display(0,13,2); 
                if (joystick_button_pressed()){
                    State = case 0;
                }   
                break;
            case 6:
                clean_display();
                draw_text_buffer("Desligar", 10,0);
                draw_text_buffer("Sim", 5, 10);
                draw_text_buffer("Nao",5,20);
                draw_circle_display(0,13,2);
                update_display(); 
                if (joystick_button_pressed()){
                    State = case 0;
                }
                break;
            case 7:
                clean_display();
                draw_text_buffer("Desligar", 10,0);
                draw_text_buffer("Sim", 5, 10);
                draw_text_buffer("Nao",5,20);
                draw_circle_display(0,23,2);
                update_display(); 
                if (joystick_button_pressed()){
                    State = case 3;
                }
                break;
        }
        
        // Envia para o display físico
        update_display();
        sleep_ms(100);
    }
    
    return 0;
}