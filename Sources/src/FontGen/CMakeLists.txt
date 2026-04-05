# FontGen - Генератор шрифтов
# Зависит от: BugSlay, Image, StreamIO, Misc, Common, Formats

cmake_minimum_required(VERSION 3.0)

project(FontGen LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB SOURCES "*.cpp")
FILE(GLOB HEADERS "*.h")

add_executable(FontGen ${SOURCES} ${HEADERS})

# Прекомпилированные заголовки
set_target_properties(FontGen PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
)

set_source_files_properties(StdAfx.cpp PROPERTIES
    COMPILE_FLAGS "/YcStdAfx.h"
)

# Определения для Windows
target_compile_definitions(FontGen PRIVATE
    WIN32
    _CONSOLE
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _WIN32_WINNT=0x0400
    WINVER=0x0400
)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../BugSlay
    ${CMAKE_CURRENT_SOURCE_DIR}/../Image
    ${CMAKE_CURRENT_SOURCE_DIR}/../StreamIO
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/../Common
    ${CMAKE_CURRENT_SOURCE_DIR}/../Formats
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(FontGen
    Image
    StreamIO_STATIC
    Common
    Formats
    Misc
    BugSlay
    LibPng
    zlib
    kernel32.lib
    user32.lib
    gdi32.lib
    winspool.lib
    comdlg32.lib
    advapi32.lib
    shell32.lib
    ole32.lib
    oleaut32.lib
    uuid.lib
)
