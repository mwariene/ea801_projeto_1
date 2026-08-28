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
    init_display();
    return 0;

    display_text("Menu", 3, 2)
    write_pixel(2,3)
}