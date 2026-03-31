# Минимальная версия CMake
cmake_minimum_required(VERSION 2.4)

# Название проекта
project(GFX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB GFX_SOURCE "*.cpp")
FILE(GLOB GFX_HEADERS "*.h")





# прописать еще Directx 8




add_definitions( -D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)

add_definitions( -D_WIN32_WINNT=0x0501)

add_library(GFX STATIC ${GFX_SOURCE} ${GFX_HEADERS} GFX.rc)

# DLL
add_library(GFX SHARED ${GFX_SOURCE} ${GFX_HEADERS} GFX.rc )
