# Минимальная версия CMake
cmake_minimum_required(VERSION 2.4)

# Название проекта
project(Main)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB MAIN_SOURCE "*.cpp")
FILE(GLOB MAIN_HEADERS "*.h")

add_definitions( -D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)
add_definitions( -D_WIN32_WINNT=0x0501)

add_library(Main STATIC ${MAIN_SOURCE} ${MAIN_HEADERS})

