# Минимальная версия CMake
cmake_minimum_required(VERSION 2.4)

# Название проекта
project(StreamIO)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB STREAMIO_SOURCE "*.cpp")
FILE(GLOB STREAMIO_HEADERS "*.h")

add_definitions( -D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)
add_definitions( -D_WIN32_WINNT=0x0501)

# LIB
add_library(StreamIO STATIC ${STREAMIO_HEADERS} ${STREAMIO_SOURCE})
# DLL
add_library(StreamIO SHARED ${STREAMIO_HEADERS} ${STREAMIO_SOURCE})
# (Опционально) Устанавливаем версию библиотеки
set_target_properties(StreamIO  PROPERTIES VERSION 1.0.0 SOVERSION 1)



