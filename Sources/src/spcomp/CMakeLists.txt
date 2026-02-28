# Минимальная версия CMake
cmake_minimum_required(VERSION 2.4)

# Название проекта
project(SpComp)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB SPCOMP_SOURCE "*.cpp")
FILE(GLOB SPCOMP_HEADERS "*.h")

add_definitions( -D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)
add_definitions( -D_WIN32_WINNT=0x0501)

add_executable(SpComp   ${SPCOMP_SOURCE} ${SPCOMP_HEADERS})

