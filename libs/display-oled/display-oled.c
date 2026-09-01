#include "display-oled.h"
#include <stdio.h>
#include <string.h>
#include "ssd1306.h"
#include "hardware/i2c.h"
#include "../bitdoglab.h"

// Buffer e área de renderização globais
static uint8_t ssd[ssd1306_buffer_length];
static struct render_area frame_area;

void display_init(void) {
    // Inicializa I2C
    i2c_init(OLED_I2C, ssd1306_i2c_clock * 1000);
    gpio_set_function(OLED_SDA, GPIO_FUNC_I2C);
    gpio_set_function(OLED_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(OLED_SDA);
    gpio_pull_up(OLED_SCL);
    
    sleep_ms(100); // Delay para estabilizar
    
    // Verifica se o display está respondendo
    uint8_t test = 0;
    int ret = i2c_read_blocking(OLED_I2C, ssd1306_i2c_addr, &test, 1, false);
    printf("I2C probe ret = %d\n", ret);
    
    if (ret < 0) {
        printf("OLED NAO RESPONDE NO ENDERECO 0x%02X\n", ssd1306_i2c_addr);
        // Não trava o programa, apenas avisa
    } else {
        printf("OLED ENCONTRADO!\n");
        ssd1306_init();
    }
    
    // Configura área de renderização
    frame_area.start_column = 0;
    frame_area.end_column = ssd1306_width - 1;
    frame_area.start_page = 0;
    frame_area.end_page = ssd1306_n_pages - 1;
    
    calculate_render_area_buffer_length(&frame_area);
    
    // Limpa o display
    clean_display();
    sleep_ms(100);
}

void clean_display(void) {
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
}

void write_text_display(const char *text, int x, int y) {
    ssd1306_draw_string(ssd, x, y, (char*)text);
    render_on_display(ssd, &frame_area);
}

void clear_buffer(void) {
    memset(ssd, 0, ssd1306_buffer_length);
}

void draw_text_buffer(const char *text, int x, int y) {
    ssd1306_draw_string(ssd, x, y, (char*)text);
}

void update_display(void) {
    render_on_display(ssd, &frame_area);
}