#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "display-oled.h"
// REMOVA: #include "hardware/adc.h"

int main(){
    stdio_init_all();
    
    display_init();
    clean_display();

    while (true) {
        // Prepara a memória com o texto
        draw_text_buffer("Velocidade", 0, 0);
        draw_text_buffer("20km/h",50,10);
        draw_text_buffer("Velocidade Max", 0, 25);
        draw_text_buffer("100km/h",50,35);
        draw_text_buffer("Distancia", 0, 50);
        draw_text_buffer("100km/h",50,60);
        
        // Envia para o display físico
        update_display();
        sleep_ms(100);
    }
    
    return 0;
}