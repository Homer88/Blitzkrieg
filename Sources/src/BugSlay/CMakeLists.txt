# BugSlay - Базовая библиотека утилит с DirectX 8
# Согласно BugSlay.dsp - это DLL (Dynamic-Link Library)
# Зависимости: нет (базовая библиотека)

cmake_minimum_required(VERSION 3.5)

project(BugSlay LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Исходные файлы
FILE(GLOB BUGSLAY_SOURCES "*.cpp")
FILE(GLOB BUGSLAY_HEADERS "*.h")

# Исключаем файлы с ассемблером (не совместимы с x64)
list(REMOVE_ITEM BUGSLAY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/DumbPow2Alloc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MemorySystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/STLDebugTerminate.cpp
)

# BugSlay - это DLL согласно .dsp файлу
add_library(BugSlay SHARED
    ${BUGSLAY_SOURCES}
    ${BUGSLAY_HEADERS}
)

# Прекомпилированные заголовки
set_target_properties(BugSlay PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
)

set_source_files_properties(StdAfx.cpp PROPERTIES
    COMPILE_FLAGS "/YcStdAfx.h"
)

# Определения для Windows (согласно .dsp файлу)
target_compile_definitions(BugSlay PRIVATE
    WIN32
    _WINDOWS
    BUGSLAYER_EXPORTS
    _DONT_LOAD_SINGLETONS
    _DONT_LOAD_STREAMIO
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    _WIN32_WINNT=0x0400
    WINVER=0x0400
)

# Подключение заголовочных файлов
target_include_directories(BugSlay PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
)

# DirectX 8 SDK
set(DIRECTX8_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../sdk/directx/include")
set(DIRECTX8_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../sdk/directx/lib")

if(EXISTS "${DIRECTX8_INCLUDE_DIR}")
    target_include_directories(BugSlay PRIVATE ${DIRECTX8_INCLUDE_DIR})
    target_link_directories(BugSlay PRIVATE ${DIRECTX8_LIB_DIR})
    target_link_libraries(BugSlay PRIVATE d3d8.lib d3dx8.lib dxguid.lib)
endif()

# Системные библиотеки Windows (согласно .dsp)
target_link_libraries(BugSlay PRIVATE
    comctl32.lib
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
