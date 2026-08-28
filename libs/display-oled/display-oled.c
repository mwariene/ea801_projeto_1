#include "display-oled.h"
#include <stdio.h>
#include <string.h>
#include "inc/ssd1306.h"
#include "hardware/i2c.h"

#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 3

void init_display() {
    stdio_init_all();

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

    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
}