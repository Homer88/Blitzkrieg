# Input - Библиотека ввода
# Зависит от: BugSlay, Misc, StreamIO

cmake_minimum_required(VERSION 3.0)

project(Input LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB SOURCES "*.cpp")
FILE(GLOB HEADERS "*.h")

add_library(Input STATIC ${SOURCES} ${HEADERS})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../BugSlay
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/../StreamIO
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Прекомпилированные заголовки
set_target_properties(Input PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
)

set_source_files_properties(StdAfx.cpp PROPERTIES
    COMPILE_FLAGS "/YcStdAfx.h"
)

# Определения для Windows
target_compile_definitions(Input PRIVATE
    WIN32
    _LIB
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _WIN32_WINNT=0x0400
    WINVER=0x0400
)

target_link_libraries(Input
    StreamIO_STATIC
    Misc
    BugSlay
    zlib
)
