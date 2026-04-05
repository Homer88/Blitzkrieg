# BuildVersion - Утилита версии сборки
# Консольное приложение
# Зависит от: Misc

cmake_minimum_required(VERSION 3.5)

project(BuildVersion LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Исходные файлы
FILE(GLOB SOURCES "*.cpp")
FILE(GLOB HEADERS "*.h")

add_executable(BuildVersion ${SOURCES} ${HEADERS})

# Прекомпилированные заголовки
set_target_properties(BuildVersion PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
)

set_source_files_properties(StdAfx.cpp PROPERTIES
    COMPILE_FLAGS "/YcStdAfx.h"
)

# Определения для Windows
target_compile_definitions(BuildVersion PRIVATE
    WIN32
    _CONSOLE
    _MBCS
    _DEVVERSION
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    _WIN32_WINNT=0x0400
    WINVER=0x0400
)

# Подключение заголовочных файлов
target_include_directories(BuildVersion PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/Misc
)

# Системные библиотеки Windows
target_link_libraries(BuildVersion PRIVATE
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
