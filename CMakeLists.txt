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

add_executable(codigo_base
    codigo_base.c
    src/led_rgb.c
    src/led_matrix.c
)

pico_set_program_name(codigo_base "codigo_base")
pico_set_program_version(codigo_base "0.1")

# Comentado para não dar erro enquanto você não tiver o arquivo .pio criado
# pico_generate_pio_header(codigo_base ${CMAKE_CURRENT_LIST_DIR}/blink.pio)

pico_enable_stdio_uart(codigo_base 0)
pico_enable_stdio_usb(codigo_base 1)

pico_generate_pio_header(codigo_base ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)

# Lista de bibliotecas unificada e corrigida (sem o 's' em shardware_i2c)
target_link_libraries(codigo_base
        pico_stdlib
        hardware_adc
        hardware_i2c
        hardware_pio
        hardware_pwm
        hardware_clocks
)

target_include_directories(codigo_base PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(codigo_base)