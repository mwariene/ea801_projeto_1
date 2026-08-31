    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <ctype.h>
    #include "pico/stdlib.h"
    #include "pico/binary_info.h"
    #include "..\libs\display-oled\ssd1306.h"
    #include "hardware/i2c.h"
    #include "../libs/display-oled/display-oled.h"

    // parte comum a todos os códigos de display
    int main(){
        stdio_init_all();
        init_display();
        clear_display();

        // 1. Prepara a memória com os desenhos
        display_text("Menu", 3, 2);
        write_pixel(2, 3, true);
        
        // 2. Envia a memória preparada para o display físico!
        display_update(); 

        // 3. Laço infinito para manter o programa rodando
        while (true) {
            sleep_ms(100);
        }
    
    return 0;
}