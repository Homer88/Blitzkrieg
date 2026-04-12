# Net - Сетевая библиотека (DLL + STATIC)
# Зависит от: BugSlay, Misc, StreamIO

cmake_minimum_required(VERSION 2.4)

project(Net)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB NET_SOURCE "*.cpp")
FILE(GLOB NET_HEADERS "*.h")

add_definitions(-D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)
add_definitions(-D_WIN32_WINNT=0x0501)

# Создаем STATIC библиотеку
add_library(Net STATIC ${NET_SOURCE} ${NET_HEADERS} Net.rc)

# Создаем DLL
add_library(Net_DLL SHARED ${NET_SOURCE} ${NET_HEADERS} Net.rc)

# Установка имени выходного файла
set_target_properties(Net PROPERTIES
    OUTPUT_NAME "Net_static"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)

set_target_properties(Net_DLL PROPERTIES
    OUTPUT_NAME "Net"
    PREFIX ""
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

# Прекомпилированные заголовки для STATIC
set_target_properties(Net PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
)

# Прекомпилированные заголовки для DLL
set_target_properties(Net_DLL PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
)

set_source_files_properties(StdAfx.cpp PROPERTIES
    COMPILE_FLAGS "/YcStdAfx.h"
)

# Определения для Windows
target_compile_definitions(Net PRIVATE
    WIN32
    _LIB
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _WIN32_WINNT=0x0501
    WINVER=0x0501
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
)

target_compile_definitions(Net_DLL PRIVATE
    WIN32
    _WINDOWS
    _USRDLL
    NET_EXPORTS
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _WIN32_WINNT=0x0501
    WINVER=0x0501
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
)

# Подключение заголовочных файлов
target_include_directories(Net PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/BugSlay
    ${CMAKE_SOURCE_DIR}/Misc
    ${CMAKE_SOURCE_DIR}/StreamIO
)

target_include_directories(Net_DLL PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/BugSlay
    ${CMAKE_SOURCE_DIR}/Misc
    ${CMAKE_SOURCE_DIR}/StreamIO
)

# Подключение зависимостей
target_link_libraries(Net
    BugSlay
    StreamIO_STATIC
    Misc
    ws2_32.lib
    winmm.lib
)

target_link_libraries(Net_DLL
    BugSlay
    StreamIO
    Misc
    ws2_32.lib
    winmm.lib
)
