
#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "../bitdoglab.h"

void init_display() {
    stdio_init_all();

void clean_display(void);

void display_init(void){
    i2c_init(OLED_I2C, ssd1306_i2c_clock*1000);
    gpio_set_function(OLED_SDA, GPIO_FUNC_I2C);
    gpio_set_function(OLED_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(OLED_SDA);
    gpio_pull_up(OLED_SCL);

    // Limpar buffer antes de inicializar display
    memset(ssd_buffer, 0, ssd1306_buffer_length);
    
    ssd1306_init();

    frame_area.start_column = 0;
    frame_area.end_column = ssd1306_width - 1;
    frame_area.start_page = 0;
    frame_area.end_page = ssd1306_n_pages - 1;

    calculate_render_area_buffer_length(&frame_area);
    clean_display();
    
    sleep_ms(100);  // Delay para estabilização
}

void clean_display(void){
    memset(ssd_buffer, 0, ssd1306_buffer_length);
    render_on_display(ssd_buffer, &frame_area);
    sleep_ms(10);  // Pequeno delay
}

void write_text_display(const char *text, int x, int y){
    // Desenhar texto no buffer
    ssd1306_draw_string(ssd_buffer, x, y, (char*)text);
    // Renderizar apenas a área onde o texto foi desenhado
    render_on_display(ssd_buffer, &frame_area);
}

void clear_buffer(void) {
    memset(ssd_buffer, 0, ssd1306_buffer_length);
}

void draw_text_buffer(const char *text, int x, int y) {
    ssd1306_draw_string(ssd_buffer, x, y, (char*)text);
}

void update_display(void) {
    render_on_display(ssd_buffer, &frame_area);
}
