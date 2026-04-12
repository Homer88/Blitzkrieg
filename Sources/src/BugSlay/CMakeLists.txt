# BugSlay - Базовая библиотека утилит с DirectX 8 (DLL + STATIC)
# Зависимости: нет (базовая библиотека)

cmake_minimum_required(VERSION 3.5)

project(BugSlay LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Исходные файлы
FILE(GLOB BUGSLAY_SOURCES "*.cpp")
FILE(GLOB BUGSLAY_HEADERS "*.h")





# BugSlay - это DLL согласно .dsp файлу (основная)
add_library(BugSlay SHARED
    ${BUGSLAY_SOURCES}
    ${BUGSLAY_HEADERS}
)

# BugSlay_STATIC - статическая версия (для внутренней линковки)
add_library(BugSlay_STATIC STATIC
    ${BUGSLAY_SOURCES}
    ${BUGSLAY_HEADERS}
)

# Прекомпилированные заголовки для DLL
set_target_properties(BugSlay PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
    OUTPUT_NAME "BugSlay"
    PREFIX ""
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)


# Прекомпилированные заголовки для STATIC
set_target_properties(BugSlay_STATIC PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
    OUTPUT_NAME "BugSlay_static"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)

set_source_files_properties(StdAfx.cpp PROPERTIES
    COMPILE_FLAGS "/YcStdAfx.h"
)

# Определения для DLL
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

# Определения для STATIC
target_compile_definitions(BugSlay_STATIC PRIVATE
    WIN32
    _WINDOWS
    _LIB
    BUGSLAYER_EXPORTS
    _DONT_LOAD_SINGLETONS
    _DONT_LOAD_STREAMIO
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    _WIN32_WINNT=0x0400
    WINVER=0x0400
)

# Подключение заголовочных файлов — DLL
target_include_directories(BugSlay PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
)

# Подключение заголовочных файлов — STATIC
target_include_directories(BugSlay_STATIC PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
)

# DirectX 8 SDK
set(DIRECTX8_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../sdk/directx/include")
set(DIRECTX8_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../sdk/directx/lib")

if(EXISTS "${DIRECTX8_INCLUDE_DIR}")
    target_include_directories(BugSlay PRIVATE ${DIRECTX8_INCLUDE_DIR})
    target_include_directories(BugSlay_STATIC PRIVATE ${DIRECTX8_INCLUDE_DIR})
    target_link_directories(BugSlay PRIVATE ${DIRECTX8_LIB_DIR})
    target_link_directories(BugSlay_STATIC PRIVATE ${DIRECTX8_LIB_DIR})
    target_link_libraries(BugSlay PRIVATE d3d8.lib d3dx8.lib dxguid.lib)
    target_link_libraries(BugSlay_STATIC PRIVATE d3d8.lib d3dx8.lib dxguid.lib)
endif()

# Системные библиотеки Windows — DLL
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

# Системные библиотеки Windows — STATIC
target_link_libraries(BugSlay_STATIC PRIVATE
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
