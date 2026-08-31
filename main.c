#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "display-oled.h"

int main(){
    stdio_init_all();
    
    display_init();
    clean_display();

    while (true) {
        // Prepara a memória com o texto
        draw_text_buffer("Menu", 0, 0);
        
        // Envia para o display físico
        update_display();
        sleep_ms(100);
    }
    
    return 0;
}