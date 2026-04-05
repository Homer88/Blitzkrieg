# Минимальная версия CMake
cmake_minimum_required(VERSION 3.5)

# Название проекта
project(Scene)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB SCENE_SOURCE "*.cpp")
FILE(GLOB SCENE_HEADERS "*.h")

add_definitions( -D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)
add_definitions( -D_WIN32_WINNT=0x0501)

add_library(Scene STATIC ${SCENE_HEADERS} ${SCENE_SOURCE})

