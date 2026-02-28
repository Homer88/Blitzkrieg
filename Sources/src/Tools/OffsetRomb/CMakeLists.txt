# Минимальная версия CMake
cmake_minimum_required(VERSION 2.4)

# Название проекта
project(OffsetRomb)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB OFFSETROMB_SOURCE "*.cpp")
FILE(GLOB OFFSETROMB_HEADERS "*.h")

add_definitions( -D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)
add_definitions( -D_WIN32_WINNT=0x0501)

add_executable(OffsetRomb   ${OFFSETROMB_SOURCE} ${OFFSETROMB_HEADERS} )

