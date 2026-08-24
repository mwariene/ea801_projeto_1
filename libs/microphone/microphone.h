#ifndef microphone_h
#define microphone_h

#include "pico/stdlib.h"

#define mic_channel 2
#define mic_pin (26+mic_channel) // GPIO26 for ADC0, GPIO27 for ADC1, etc.
#define samples 200

void microphone_init(void);
void microphone_start_sampling(void);
void microphone_stop_sampling(void);
void microphone_process_samples(void);
float microphone_get_power(void);
uint8_t microphone_get_level(void);
#endif