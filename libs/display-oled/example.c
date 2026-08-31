#include "display_oled.h"

int main(){
    init_display();

    write_text_display("Hello, World!", 0, 0);

    while (true){
        tight_loop();
    }
}