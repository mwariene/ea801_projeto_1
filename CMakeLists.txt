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
set(PICO_PLATFORM rp2350-arm-s CACHE STRING "Platform type")
set(PICO_BOARD pico2_w CACHE STRING "Board type")

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Inicializa o SDK do Pico
include(pico_sdk_import.cmake)

# Define o projeto
project(main C CXX ASM)

# Inicializa o SDK
pico_sdk_init()

# Adiciona o executável APENAS com main.c e os módulos necessários
add_executable(main
    main.c
    libs/display-oled/display-oled.c
    libs/display-oled/ssd1306.c
    libs/inputs/buttons.c
)

# Configurações do programa
pico_set_program_name(main "main")
pico_set_program_version(main "0.1")

# Habilita USB para debug
pico_enable_stdio_uart(main 0)
pico_enable_stdio_usb(main 1)

# Bibliotecas necessárias (apenas as que você está usando)
target_link_libraries(main
    pico_stdlib
    hardware_i2c
)

# Diretórios de inclusão
target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/libs
    ${CMAKE_CURRENT_LIST_DIR}/libs/display-oled
)

# Saídas extras
pico_add_extra_outputs(main)