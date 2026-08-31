#include "display-oled.h"
#include <stdio.h>
#include <string.h>
#include "ssd1306.h"
#include "hardware/i2c.h"
<<<<<<< Updated upstream

#define I2C_SDA_PIN 2
#define I2C_SCL_PIN 3

// 1. Criação do Buffer e da Área de Renderização como GLOBAIS no módulo
static uint8_t ssd[ssd1306_buffer_length];
static struct render_area frame_area;

void init_display() {
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    uint8_t test = 0;
    int ret = i2c_read_blocking(i2c1, 0x3C, &test, 1, false);
    printf("I2C probe ret = %d\n", ret);

    if (ret < 0) {
        while (true) {
            printf("OLED NAO RESPONDE NO ENDERECO 0x3C\n");
            sleep_ms(500);
        }
    }

    ssd1306_init();

    // Configura a área de renderização global
=======
#include "../bitdoglab.h"

// Criação do Buffer e da Área de Renderização como GLOBAIS no módulo
static uint8_t ssd[ssd1306_buffer_length];
static struct render_area frame_area;

void display_init(void) {
    i2c_init(OLED_I2C, ssd1306_i2c_clock * 1000);
    gpio_set_function(OLED_SDA, GPIO_FUNC_I2C);
    gpio_set_function(OLED_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(OLED_SDA);
    gpio_pull_up(OLED_SCL);

    // Declaração da variável 'ret' que estava faltando
    uint8_t test = 0;
    int ret = i2c_read_blocking(OLED_I2C, ssd1306_i2c_addr, &test, 1, false);
    printf("I2C probe ret = %d\n", ret);

    if (ret < 0) {
        while (true) {
            printf("OLED NAO RESPONDE NO ENDERECO 0x3C\n");
            sleep_ms(500);
        }
    }

    ssd1306_init();

    // Configura a área de renderização global (exemplo 128x128 ou 128x64)
>>>>>>> Stashed changes
    frame_area.start_column = 0;
    frame_area.end_column = ssd1306_width - 1;
    frame_area.start_page = 0;
    frame_area.end_page = ssd1306_n_pages - 1;

    calculate_render_area_buffer_length(&frame_area);
<<<<<<< Updated upstream

    // Limpa o buffer inicial e manda para a tela
    memset(ssd, 0xFF, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
}

// 2. Função EXCLUSIVA para atualizar a tela
void display_update() {
    render_on_display(ssd, &frame_area);
}

// 3. Funções de desenho agora APENAS modificam o buffer 'ssd'
void display_text(const char *text, int x, int y){
    ssd1306_draw_string(ssd, x, y, (char *)text);
}

void clear_display(){
    memset(ssd, 0, ssd1306_buffer_length); // Apenas zera a memória
}

void write_pixel(int x, int y, bool on){
    // O parâmetro 'on' permite tanto desenhar (true) quanto apagar (false) o pixel
    ssd1306_set_pixel(ssd, x, y, on);
=======
    
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
>>>>>>> Stashed changes
}