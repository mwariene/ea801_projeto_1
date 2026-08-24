#include <stdio.h>
#include "pico/stdlib.h"
#include "buzzer.h"

int main(){
    stdio_init_all();
    buzzer_init();

    while(true){
        printf("Beeping the buzzer for 500 ms...\n");
        buzzer_beep(500);

        buzzer_play_tone(1000, 1000); // Play a 1 kHz tone for 1 second
        
        sleep_ms(1000); // Wait for 1 second before the next beep
    }
}