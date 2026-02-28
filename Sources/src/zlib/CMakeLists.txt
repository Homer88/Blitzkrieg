# Минимальная версия CMake
cmake_minimum_required(VERSION 2.0)

# Название проекта
project(zlib) 

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)
FILE(GLOB ZIP_LIB_SOURCE "*.c")
FILE(GLOB ZIP_LIB_HEADERS "*.h")

#add_executable(zlib ${ZIP_LIB_SOURCE} ${ZIP_LIB_HEADERS})
add_library(zlib STATIC ${ZIP_LIB_SOURCE} ${ZIP_LIB_HEADERS})