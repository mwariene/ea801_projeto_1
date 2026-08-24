#include "display_oled.h"

int main(){
    display_init();

    write_text_display("Hello, World!", 0, 0);

    while (true){
        tight_loop();
    }
}