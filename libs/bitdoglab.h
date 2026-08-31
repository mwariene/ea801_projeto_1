#ifndef BITDOGLAB_H
#define BITDOGLAB_H

// DEFINIÇÕES DE PINOS

// LED RGB
#define LED_RED     13
#define LED_GREEN   11
#define LED_BLUE    12

// Botões (Entradas com Resistor Pull-Up interno)
#define button_A    5
#define button_B    6
#define button_C    10

// Joystick (ADC)
#define joy_x       1   // GPIO27
#define joy_y       0   // GPIO26
#define joy_sw      22

// OLED 1107 (I2C1)
#define OLED_I2C    i2c1
#define OLED_SDA    2
#define OLED_SCL    3
#define OLED_ADDR   0x3C

// Matriz de LEDs (5x5)
#define NEOPIXEL_PIN  7

#endif