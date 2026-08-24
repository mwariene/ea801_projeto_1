#include "microphone.h"
#include "hardware/adc.h"
#include "hardware/dma.h"
#include <math.h>

#define ADC_CLOCK_DIV 96.f
#define ADC_ADJUST(x) (x * 3.3f / (1 << 12u) - 1.65f)

static uint dma_channel;
static dma_channel_config dma_config;
static uint16_t adc_buffer[samples];

void microphone_init(void) {
    adc_gpio_init(mic_pin);
    adc_init();
    adc_select_input(mic_channel);

    adc_fifo_setup(true, true, 1, false, false);
    adc_set_clkdiv(ADC_CLOCK_DIV);

    dma_channel = dma_claim_unused_channel(true);
    dma_config = dma_channel_get_default_config(dma_channel);

    channel_config_set_transfer_data_size(&dma_config, DMA_SIZE_16);
    channel_config_set_read_increment(&dma_config, false);
    channel_config_set_write_increment(&dma_config, true);
    channel_config_set_dreq(&dma_config, DREQ_ADC);
}

// Faz a amostragem completa e aguarda o término
void microphone_read(void) {
    adc_fifo_drain();

    dma_channel_configure(
        dma_channel,
        &dma_config,
        adc_buffer,
        &adc_hw->fifo,
        samples,
        true
    );

    adc_run(true);                                       // Ativa o ADC para alimentar o DMA
    dma_channel_wait_for_finish_blocking(dma_channel); // Aguarda o buffer encher
    adc_run(false);                                      // Desliga o ADC
}

// Retorna o valor RMS (tensão média do som)
float microphone_get_power(void) {
    float power = 0.0f;
    for (int i = 0; i < samples; i++) {
        float adjusted = ADC_ADJUST(adc_buffer[i]);
        power += adjusted * adjusted;
    }
    return sqrtf(power / samples); // Tensão RMS real
}

// Converte a tensão RMS em um nível de 0 a 5
uint8_t microphone_get_level(void) {
    float rms = microphone_get_power();
    uint8_t level = (uint8_t)((rms / 0.8f) * 5.0f); // Considera 0.8V RMS como volume máximo
    return (level > 5) ? 5 : level;
}