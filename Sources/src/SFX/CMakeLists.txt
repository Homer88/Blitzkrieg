# Минимальная версия CMake
cmake_minimum_required(VERSION 3.5)

# Название проекта
project(SFX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB SFX_SOURCE "*.cpp")
FILE(GLOB SFX_HEADERS "*.h")

add_definitions(-D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)
add_definitions(-D_WIN32_WINNT=0x0501)

add_library(SFX STATIC ${SFX_SOURCE} ${SFX_HEADERS})

# Добавляем пути для заголовков (включая SDK)
target_include_directories(SFX PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/../sdk/fmod/lowlevel/inc
)

