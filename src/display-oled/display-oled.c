
#include "bitdoglab.h"

#ifndef BITDOGLAB_H
#define BITDOGLAB_H

#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"

#define I2C_SDA_PIN 14
#define I2C_SCL_PIN 15

static uint8_t ssd_buffer[ssd1306_buffer_length];
static struct render_area frame_area;

void display_init(void){
    i2c_init(i2c1, ssd1306_i2c_clock*1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    ssd1306_init();

    // Define a área padrão de renderização
    frame_area.start_column = 0;
    frame_area.end_column = ssd1306_width - 1;
    frame_area.start_page = 0;
    frame_area.end_page = ssd1306_n_pages - 1;

    calculate_render_area_buffer_length(&frame_area);
    clean_display();
}

void clean_display(void){
    memset(ssd_buffer, 0, ssd1306_buffer_length);
    render_on_display(ssd_buffer, &frame_area);
}

void write_text_display(const char *text, int x, int y){
    ssd1306_draw_string(ssd_buffer, x, y, (char*)text);
    render_on_display(ssd_buffer, &frame_area);
}