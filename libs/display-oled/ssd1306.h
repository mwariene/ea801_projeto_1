<<<<<<< Updated upstream
#include "ssd1306_i2c.h"
extern void calculate_render_area_buffer_length(struct render_area *area);
extern void ssd1306_send_command(uint8_t cmd);
extern void ssd1306_send_command_list(uint8_t *ssd, int number);
extern void ssd1306_send_buffer(uint8_t ssd[], int buffer_length);
extern void ssd1306_init();
extern void ssd1306_scroll(bool set);
extern void render_on_display(uint8_t *ssd, struct render_area *area);
extern void ssd1306_set_pixel(uint8_t *ssd, int x, int y, bool set);
extern void ssd1306_draw_line(uint8_t *ssd, int x_0, int y_0, int x_1, int y_1, bool set);
extern void ssd1306_draw_char(uint8_t *ssd, int16_t x, int16_t y, uint8_t character);
extern void ssd1306_draw_string(uint8_t *ssd, int16_t x, int16_t y, char *string);
extern void ssd1306_command(ssd1306_t *ssd, uint8_t command);
extern void ssd1306_config(ssd1306_t *ssd);
extern void ssd1306_init_bm(ssd1306_t *ssd, uint8_t width, uint8_t height, bool external_vcc, uint8_t address, i2c_inst_t *i2c);
extern void ssd1306_send_data(ssd1306_t *ssd);
extern void ssd1306_draw_bitmap(ssd1306_t *ssd, const uint8_t *bitmap);
=======
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
void ssd1306_draw_pixel(uint8_t *buffer, int x, int y, bool set);

#endif
>>>>>>> Stashed changes
