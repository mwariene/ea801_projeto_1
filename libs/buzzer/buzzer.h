#ifndef BUZZER_H
#define BUZZER_H

#include "pico/stdlib.h"

#define buzzer_pin 21

void buzzer_init(void);
void buzzer_on(void);
void buzzer_off(void);
void buzzer_beep(uint32_t duration_ms);
void buzzer_play_tone(uint32_t frequency, uint32_t duration_ms);

#endif