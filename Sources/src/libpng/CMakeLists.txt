# Минимальная версия CMake
cmake_minimum_required(VERSION 2.4)

# Название проекта
project(LibPng)

# Установка стандарта C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

FILE(GLOB LIBPNG_SOURCE "*.c")
FILE(GLOB LIBPNG_HEADERS "*.h")

# Исключаем файлы с ассемблером (не совместимы с x64)
# pngvcrd.c и pnggccrd.c содержат x86 SIMD оптимизации
list(REMOVE_ITEM LIBPNG_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/pngvcrd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pnggccrd.c
)

add_definitions(-D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)

add_library(LibPng STATIC ${LIBPNG_SOURCE} ${LIBPNG_HEADERS})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../zlib
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Линкуем с zlib
target_link_libraries(LibPng zlib)
