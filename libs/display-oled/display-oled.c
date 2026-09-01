#include "display-oled.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

// Desenha um pixel individual
void draw_pixel_display(int x, int y, bool set) {
    ssd1306_draw_pixel(ssd, x, y, set);
}

// Desenha uma linha (algoritmo de Bresenham)
void draw_line_display(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    while (true) {
        ssd1306_draw_pixel(ssd, x1, y1, true);
        
        if (x1 == x2 && y1 == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// Desenha um retângulo
void draw_rect_display(int x1, int y1, int x2, int y2) {
    // Linhas horizontais
    for (int x = x1; x <= x2; x++) {
        ssd1306_draw_pixel(ssd, x, y1, true);
        ssd1306_draw_pixel(ssd, x, y2, true);
    }
    // Linhas verticais
    for (int y = y1; y <= y2; y++) {
        ssd1306_draw_pixel(ssd, x1, y, true);
        ssd1306_draw_pixel(ssd, x2, y, true);
    }
}

// Desenha um círculo
void draw_circle_display(int center_x, int center_y, int radius) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x*x + y*y <= radius*radius) {
                ssd1306_draw_pixel(ssd, center_x + x, center_y + y, true);
            }
        }
    }
}