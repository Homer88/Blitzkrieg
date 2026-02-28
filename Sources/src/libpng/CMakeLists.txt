# Минимальная версия CMake
cmake_minimum_required(VERSION 2.4)

# Название проекта
project(LibPng)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB LIBPNG_SOURCE "*.c")
FILE(GLOB LIBPNG_HEADERS "*.h")

add_definitions( -D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)
add_definitions( -D_WIN32_WINNT=0x0501)

add_library(LibPng STATIC ${LIBPNG_SOURCE} ${LIBPNG_HEADERS})

