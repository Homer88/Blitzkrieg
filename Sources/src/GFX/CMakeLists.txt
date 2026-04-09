# GFX - Графическая библиотека с DirectX 8
# Зависимости: BugSlay, Image, Misc, StreamIO, Formats
# Тип: DLL (Dynamic-Link Library)

cmake_minimum_required(VERSION 2.4)

project(GFX LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Исходные файлы из GFX.dsp
FILE(GLOB GFX_SOURCE "*.cpp")
FILE(GLOB GFX_HEADERS "*.h")

# Ресурсы и def файл
set(GFX_RESOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/GFX.rc
    ${CMAKE_CURRENT_SOURCE_DIR}/GFX.def
)

# DLL библиотека
add_library(GFX SHARED ${GFX_SOURCE} ${GFX_HEADERS} ${GFX_RESOURCES})

# Определение файла .def для экспорта
set_target_properties(GFX PROPERTIES
    LINK_FLAGS "/DEF:\"${CMAKE_CURRENT_SOURCE_DIR}/GFX.def\""
)

# Прекомпилированные заголовки
set_target_properties(GFX PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
)

set_source_files_properties(StdAfx.cpp PROPERTIES
    COMPILE_FLAGS "/YcStdAfx.h"
)

# Определения для Windows 98/2000
target_compile_definitions(GFX PRIVATE
    WIN32
    _WINDOWS
    _USRDLL
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    _WIN32_WINNT=0x0501
    WINVER=0x0501
)

# Определения для разных конфигуций сборки
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(GFX PRIVATE _FINALRELEASE NDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(GFX PRIVATE _DEBUG _DO_ASSERT_SLOW _DO_CHECKED_CAST _STL_RANGE_CHECK)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    target_compile_definitions(GFX PRIVATE _BETARELEASE NDEBUG _DO_ASSERT_SLOW)
endif()

# Пути include
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
endif()

if(EXISTS "${DIRECTX8_LIB_DIR}")
    LINK_DIRECTORIES(${DIRECTX8_LIB_DIR})
endif()

# Библиотеки из GFX.dsp: d3d8.lib dxguid.lib kernel32.lib user32.lib gdi32.lib winspool.lib 
# comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comsupp.lib
target_link_libraries(GFX
    # DirectX 8
    d3d8
    dxguid

    # Системные библиотеки Windows
    kernel32
    user32
    gdi32
    winspool
    comdlg32
    advapi32
    shell32
    ole32
    oleaut32
    uuid
    odbc32
    odbccp32

    # COM Support
    comsupp
)

# Зависимости от других модулей
target_link_libraries(GFX Image Misc BugSlay)
