# Input - Библиотека ввода (DLL + STATIC)
# Зависит от: BugSlay, Misc, StreamIO

cmake_minimum_required(VERSION 3.0)

project(Input LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB SOURCES "*.cpp")
FILE(GLOB HEADERS "*.h")

# Создаем STATIC библиотеку
add_library(Input STATIC ${SOURCES} ${HEADERS})

# Создаем DLL
add_library(Input_DLL SHARED ${SOURCES} ${HEADERS})

# Установка имени выходного файла
set_target_properties(Input PROPERTIES
    OUTPUT_NAME "Input_static"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)

set_target_properties(Input_DLL PROPERTIES
    OUTPUT_NAME "Input"
    PREFIX ""
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../BugSlay
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/../StreamIO
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Прекомпилированные заголовки для STATIC
set_target_properties(Input PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
)

# Прекомпилированные заголовки для DLL
set_target_properties(Input_DLL PROPERTIES
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

target_compile_definitions(Input_DLL PRIVATE
    WIN32
    _WINDOWS
    _USRDLL
    INPUT_EXPORTS
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

target_link_libraries(Input_DLL
    StreamIO
    Misc
    BugSlay
    zlib
)
