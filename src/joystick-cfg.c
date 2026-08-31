#include "../libs/joystick/joystick.h"
#include "../libs/display-oled/display-oled.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

int joystick_main() {
    stdio_init_all();

while(1){
    uint16_t x = joystick_read_x();
    uint16_t y = joystick_read_y();

    clear_display();

    /* para calcular a posicao do pixel de acrodo com o joystick
    precisa fazer uma regra de tres para converter o sinal adc em pixels
     128 pixels de largura -> último pixel é o 127
     64 pixels de altura -> último pixel é o 63
     pos = (leitura_joy * posicao_max-tela)/4095
     4095 -> valor max que o adc consegue ler */
    int pos_x = (x*127)/4095; // arrumar de acordo com o tamanho do display
    int pos_y = (y*127)/4095;

    write_pixel(pos_x, pos_y, true);

    display_update();

    sleep_ms(100);
    }
    return 0;
}