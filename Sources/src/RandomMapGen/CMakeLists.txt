# RandomMapGen - Генерация случайных карт
# Статическая библиотека без внешних зависимостей
# Согласно RandomMapGen.dsp

cmake_minimum_required(VERSION 3.5)

project(RandomMapGen LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Исходные файлы
FILE(GLOB RANDOMMAPGEN_SOURCES "*.cpp")
FILE(GLOB RANDOMMAPGEN_HEADERS "*.h")

add_library(RandomMapGen STATIC
    ${RANDOMMAPGEN_SOURCES}
    ${RANDOMMAPGEN_HEADERS}
)

# Прекомпилированные заголовки
set_target_properties(RandomMapGen PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
)

set_source_files_properties(StdAfx.cpp PROPERTIES
    COMPILE_FLAGS "/YcStdAfx.h"
)

# Определения для Windows (согласно .dsp файлу)
target_compile_definitions(RandomMapGen PRIVATE
    WIN32
    _WINDOWS
    _LIB
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    _WIN32_WINNT=0x0400
    WINVER=0x0400
)

# Подключение заголовочных файлов
target_include_directories(RandomMapGen PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/BugSlay
    ${CMAKE_SOURCE_DIR}/Misc
    ${CMAKE_SOURCE_DIR}/StreamIO
    ${CMAKE_SOURCE_DIR}/Image
    ${CMAKE_SOURCE_DIR}/Formats
)
