cmake_minimum_required(VERSION 3.15)
project(example C CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Target

set(MCU "atmega328p" CACHE STRING "Target microcontroller with default value atmega328p")
set(CPU_FREQ "16000000L" CACHE STRING "MCU core frequency")

add_link_options(-mmcu=${MCU} -Wl,-Map=${PROJECT_NAME}.map)

add_compile_options(-mmcu=${MCU})

add_definitions(-DF_CPU=${CPU_FREQ})

# FreeRTOS

# This is the FreeRTOS target (see freertos/CMakeLists.txt)
set(FREERTOS_PORT "GCC_ATMEGA" CACHE STRING "" FORCE)

# Optionally select a heap implementation for dynamic memory support
# set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)

add_library(freertos_config INTERFACE)

target_include_directories(freertos_config INTERFACE freertos_config)

add_subdirectory(freertos)
set_property(TARGET freertos_kernel PROPERTY C_STANDARD 90)

# frt

add_subdirectory(frt)

# avr-libstdcpp

add_library(avr-libstdcpp INTERFACE)

target_include_directories(avr-libstdcpp SYSTEM INTERFACE avr-libstdcpp/include)

# Your project here

add_executable(${PROJECT_NAME}
    main.cpp
    uart.c
)

target_link_libraries(${PROJECT_NAME} freertos_kernel freertos_config frt avr-libstdcpp)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND avr-objcopy -j .text -j .data -O ihex ${PROJECT_NAME} ${PROJECT_NAME}.hex
    COMMAND avr-size --format avr --mcu=${MCU} ${PROJECT_NAME}
)
