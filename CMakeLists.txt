# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.0)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.0)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

set(PICO_BOARD pico2_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(codigo_base C CXX ASM)

pico_sdk_init()

# Executável principal unificado com todos os módulos e o main.c
add_executable(codigo_base
    src/display-cfg.c              # O seu arquivo principal de teste atual
    libs/display-oled/ssd1306_i2c.c # O driver básico do display
    libs/display-oled/display-oled.c # <-- ADICIONE O SEU ARQUIVO DE FUNÇÕES AQUI!
)

<<<<<<< Updated upstream
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/libs/display-oled/inc/ssd1306.h")
    target_sources(codigo_base PRIVATE libs/display-oled/display-oled.c)
    target_include_directories(codigo_base PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/libs/display-oled
    )
endif()

=======
>>>>>>> Stashed changes
pico_set_program_name(codigo_base "codigo_base")
pico_set_program_version(codigo_base "0.1")

# Comentado para não dar erro enquanto você não tiver o arquivo .pio criado
# pico_generate_pio_header(codigo_base ${CMAKE_CURRENT_LIST_DIR}/blink.pio)

pico_enable_stdio_uart(codigo_base 0)
pico_enable_stdio_usb(codigo_base 1)

pico_generate_pio_header(codigo_base ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)

# Lista de bibliotecas unificada
target_link_libraries(codigo_base
        pico_stdlib
        hardware_adc
        hardware_i2c
        hardware_pio
        hardware_pwm
        hardware_dma
        hardware_clocks
)

target_include_directories(codigo_base PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/libs
        ${CMAKE_CURRENT_LIST_DIR}/libs/buzzer
        ${CMAKE_CURRENT_LIST_DIR}/libs/display-oled
        ${CMAKE_CURRENT_LIST_DIR}/libs/joystick
        ${CMAKE_CURRENT_LIST_DIR}/libs/microphone
)

pico_add_extra_outputs(codigo_base)