#include <string.h>
#include "ssd1306.h"
#include "../bitdoglab.h"
#include "hardware/i2c.h"

// Usar a porta I2C definida no bitdoglab.h

static const uint8_t font5x7_ssd[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // Espaço
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // !
    {0x00, 0x07, 0x00, 0x07, 0x00}, // "
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, // #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // $
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // F
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
    {0x46, 0x49, 0x49, 0x49, 0x31}  // S
};

static void ssd1306_send_cmd(uint8_t cmd) {
    uint8_t buf[2] = {0x00, cmd};
    i2c_write_blocking(OLED_I2C, ssd1306_i2c_addr, buf, 2, false);
}

void ssd1306_init(void) {
    uint8_t cmds[] = {
        0xAE,           // Display OFF
        0xD5, 0x80,     // Set Display Clock Divide Ratio
        0xA8, 0x7F,     // Set Multiplex Ratio (127 para 128 linhas)
        0xD3, 0x60,     // Set Display Offset
        0x40,           // Set Start Line
        0x8D, 0x14,     // Enable Charge Pump
        0x20, 0x00,     // Set Memory Addressing Mode
        0xA0,           // Set Segment Re-map (invertido)
        0xC0,           // Set COM Output Scan Direction (invertido)
        0xDA, 0x12,     // Set COM Pins Hardware Configuration
        0x81, 0xCF,     // Set Contrast Control
        0xD9, 0xF1,     // Set Pre-charge Period
        0xDB, 0x40,     // Set VCOMH Deselect Level
        0xA4,           // Entire Display ON
        0xA6,           // Set Normal Display
        0xAF            // Display ON
    };
    for (size_t i = 0; i < sizeof(cmds); i++) {
        ssd1306_send_cmd(cmds[i]);
    }
}

void calculate_render_area_buffer_length(struct render_area *area) {
    area->buffer_length = (area->end_column - area->start_column + 1) * 
                         (area->end_page - area->start_page + 1);
}

void render_on_display(uint8_t *buffer, struct render_area *area) {
    // Para SH1107, enviar página por página
    for (uint8_t page = area->start_page; page <= area->end_page; page++) {
        // Configurar página e coluna
        ssd1306_send_cmd(0xB0 + page);  // Set page address
        ssd1306_send_cmd(0x00);         // Set lower column address
        ssd1306_send_cmd(0x10);         // Set higher column address
        
        // Preparar dados para a página
        uint16_t page_offset = page * ssd1306_width;
        uint8_t data[ssd1306_width + 1];
        data[0] = 0x40;  // Data mode
        
        // Copiar dados da página
        memcpy(&data[1], &buffer[page_offset], ssd1306_width);
        
        // Enviar página
        i2c_write_blocking(OLED_I2C, ssd1306_i2c_addr, data, ssd1306_width + 1, false);
        
        sleep_ms(1);  // Pequeno delay entre páginas
    }
}

void ssd1306_draw_pixel(uint8_t *buffer, int x, int y, bool set) {
    if (x < 0 || x >= ssd1306_width || y < 0 || y >= ssd1306_height) return;
    int page = y / 8;
    int bit = y % 8;
    int idx = page * ssd1306_width + x;

    if (set) buffer[idx] |= (1 << bit);
    else buffer[idx] &= ~(1 << bit);
}

void ssd1306_draw_char(uint8_t *buffer, int x, int y, char c) {
    // Tabela de fontes 5x7 simplificada
    static const uint8_t font_table[][5] = {
        // A-Z (índices 0-25)
        {0x7F, 0x09, 0x09, 0x09, 0x7F},  // A
        {0x7F, 0x49, 0x49, 0x49, 0x36},  // B
        {0x3E, 0x41, 0x41, 0x41, 0x22},  // C
        {0x7F, 0x41, 0x41, 0x41, 0x3E},  // D
        {0x7F, 0x49, 0x49, 0x49, 0x41},  // E
        {0x7F, 0x09, 0x09, 0x09, 0x01},  // F
        {0x3E, 0x41, 0x49, 0x49, 0x3A},  // G
        {0x7F, 0x08, 0x08, 0x08, 0x7F},  // H
        {0x00, 0x41, 0x7F, 0x41, 0x00},  // I
        {0x30, 0x40, 0x40, 0x40, 0x3F},  // J
        {0x7F, 0x08, 0x14, 0x22, 0x41},  // K
        {0x7F, 0x40, 0x40, 0x40, 0x40},  // L
        {0x7F, 0x02, 0x0C, 0x02, 0x7F},  // M
        {0x7F, 0x02, 0x04, 0x08, 0x7F},  // N
        {0x3E, 0x41, 0x41, 0x41, 0x3E},  // O
        {0x7F, 0x09, 0x09, 0x09, 0x06},  // P
        {0x3E, 0x41, 0x51, 0x21, 0x5E},  // Q
        {0x7F, 0x09, 0x19, 0x29, 0x46},  // R
        {0x26, 0x49, 0x49, 0x49, 0x32},  // S
        {0x01, 0x01, 0x7F, 0x01, 0x01},  // T
        {0x3F, 0x40, 0x40, 0x40, 0x3F},  // U
        {0x07, 0x18, 0x60, 0x18, 0x07},  // V
        {0x7F, 0x20, 0x18, 0x20, 0x7F},  // W
        {0x41, 0x22, 0x1C, 0x22, 0x41},  // X
        {0x07, 0x08, 0x70, 0x08, 0x07},  // Y
        {0x41, 0x61, 0x51, 0x49, 0x47},  // Z
        // 0-9 (índices 26-35)
        {0x3E, 0x51, 0x49, 0x45, 0x3E},  // 0
        {0x00, 0x42, 0x7F, 0x40, 0x00},  // 1
        {0x42, 0x61, 0x51, 0x49, 0x46},  // 2
        {0x21, 0x41, 0x45, 0x4B, 0x31},  // 3
        {0x18, 0x14, 0x12, 0x7F, 0x10},  // 4
        {0x27, 0x45, 0x45, 0x45, 0x39},  // 5
        {0x3C, 0x4A, 0x49, 0x49, 0x30},  // 6
        {0x01, 0x71, 0x09, 0x05, 0x03},  // 7
        {0x36, 0x49, 0x49, 0x49, 0x36},  // 8
        {0x06, 0x49, 0x49, 0x29, 0x1E},  // 9
        {0x00, 0x00, 0x00, 0x00, 0x00},  // Espaço
    };
    
    int char_idx = -1;
    
    // Mapear caractere para índice
    if (c >= 'A' && c <= 'Z') {
        char_idx = c - 'A';
    } else if (c >= 'a' && c <= 'z') {
        char_idx = c - 'a';
    } else if (c >= '0' && c <= '9') {
        char_idx = 26 + (c - '0');
    } else if (c == ' ') {
        char_idx = 36;  // Espaço
    }
    
    // Desenho de caracteres baseado na tabela de fontes
    if (char_idx >= 0 && char_idx < 37) {
        for (int col = 0; col < 5; col++) {
            uint8_t line = font_table[char_idx][col];
            for (int row = 0; row < 8; row++) {
                ssd1306_draw_pixel(buffer, x + col, y + row, (line & (1 << row)) != 0);
            }
        }
    }
}

void ssd1306_draw_string(uint8_t *buffer, int x, int y, char *text) {
    while (*text) {
        ssd1306_draw_char(buffer, x, y, *text);
        x += 6;
        text++;
    }
}