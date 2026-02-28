# Минимальная версия CMake
cmake_minimum_required(VERSION 2.4)

# Название проекта
project(Scl)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB SCL_SOURCE "*.cpp")
FILE(GLOB SCL_HEADERS "*.h")

add_definitions( -D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)
add_definitions( -D_WIN32_WINNT=0x0501)

add_executable(Scl   ${SCL_SOURCE} ${SCL_HEADERS})

