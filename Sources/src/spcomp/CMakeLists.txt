# spcomp - Sprite Composer
# Консольная утилита для работы с анимациями
# Зависимости: Formats, BugSlay, Misc, StreamIO, Image

# Минимальная версия CMake
cmake_minimum_required(VERSION 3.0)

# Название проекта
project(SpComp LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Исходные файлы
FILE(GLOB SPCOMP_SOURCE "*.cpp")
FILE(GLOB SPCOMP_HEADERS "*.h")

add_executable(SpComp ${SPCOMP_SOURCE} ${SPCOMP_HEADERS})

# Зависимости
target_link_libraries(SpComp
    Formats
    BugSlay
    Misc
    StreamIO_STATIC
    Image
    LibPng
    zlib
)

# Подключение заголовочных файлов зависимостей
target_include_directories(SpComp PRIVATE
    ${CMAKE_SOURCE_DIR}/Formats
    ${CMAKE_SOURCE_DIR}/BugSlay
    ${CMAKE_SOURCE_DIR}/Misc
    ${CMAKE_SOURCE_DIR}/StreamIO
    ${CMAKE_SOURCE_DIR}/Image
    ${CMAKE_SOURCE_DIR}/Common
)

# Определения для Windows
target_compile_definitions(SpComp PRIVATE
    WIN32
    _CONSOLE
    _MBCS
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    _WIN32_WINNT=0x0400
    WINVER=0x0400
)

# Прекомпилированные заголовки
set_target_properties(SpComp PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
)

set_source_files_properties(StdAfx.cpp PROPERTIES
    COMPILE_FLAGS "/YcStdAfx.h"
)

# Дополнительные библиотеки Windows
target_link_libraries(SpComp
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
