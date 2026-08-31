
#ifndef SSD1306_H
#define SSD1306_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Definições de constantes para SH1107 (128x128)
#define ssd1306_width 128
#define ssd1306_height 128
#define ssd1306_n_pages 16  // 128/8 = 16 páginas
#define ssd1306_buffer_length (ssd1306_width * ssd1306_n_pages)
#define ssd1306_i2c_clock 400
#define ssd1306_i2c_addr 0x3C

// Estrutura para área de renderização
struct render_area {
    uint8_t start_column;
    uint8_t end_column;
    uint8_t start_page;
    uint8_t end_page;
    uint16_t buffer_length;
};

// Declarações de funções
void ssd1306_init(void);
void render_on_display(uint8_t *buffer, struct render_area *area);
void ssd1306_draw_string(uint8_t *buffer, int x, int y, char *text);
void calculate_render_area_buffer_length(struct render_area *area);

#endif
