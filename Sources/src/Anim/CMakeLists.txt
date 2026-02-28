# Минимальная версия CMake
cmake_minimum_required(VERSION 3.0)

# Название проекта
project( Animation LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)


#AI логика 
FILE(GLOB ANIMATION_SOURCE "*.cpp")
FILE(GLOB ANIMATION_HEADERS "*.h")


add_library (ANIMATION ${ANIMATION_SOURCE} ${ANIMATION_HEADERS})
