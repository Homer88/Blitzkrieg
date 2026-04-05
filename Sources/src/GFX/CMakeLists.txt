# GFX - Графическая библиотека с DirectX 8
# Зависимости: BugSlay, Image, Misc, StreamIO, Formats

cmake_minimum_required(VERSION 3.0)

project(GFX LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB GFX_SOURCE "*.cpp")
FILE(GLOB GFX_HEADERS "*.h")

add_library(GFX STATIC ${GFX_SOURCE} ${GFX_HEADERS} GFX.rc)

# Прекомпилированные заголовки
set_target_properties(GFX PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
)

set_source_files_properties(StdAfx.cpp PROPERTIES
    COMPILE_FLAGS "/YcStdAfx.h"
)

# Определения для Windows
target_compile_definitions(GFX PRIVATE
    WIN32
    _LIB
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    _WIN32_WINNT=0x0400
    WINVER=0x0400
)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../BugSlay
    ${CMAKE_CURRENT_SOURCE_DIR}/../Image
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/../StreamIO
    ${CMAKE_CURRENT_SOURCE_DIR}/../Formats
)

# DirectX 8 SDK
set(DIRECTX8_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../sdk/directx/include")
set(DIRECTX8_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../sdk/directx/lib")

if(EXISTS "${DIRECTX8_INCLUDE_DIR}")
    INCLUDE_DIRECTORIES(${DIRECTX8_INCLUDE_DIR})
    LINK_DIRECTORIES(${DIRECTX8_LIB_DIR})
    target_link_libraries(GFX d3d8.lib d3dx8.lib dxguid.lib)
endif()

target_link_libraries(GFX Image Misc BugSlay StreamIO_STATIC LibPng zlib)
