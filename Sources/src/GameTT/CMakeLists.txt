# ============================================================
# GameTT - Основной игровой модуль (DLL + STATIC)
# Зависимости: BugSlay, Misc, StreamIO, Common, Formats, RandomMapGen
# ============================================================

cmake_minimum_required(VERSION 2.4)
project(GameTT)

# Собираем все исходные файлы
FILE(GLOB GAMETT_SOURCES "*.cpp")
FILE(GLOB GAMETT_HEADERS "*.h")

# Resource file
set(GAMETT_RC "GameTT.rc")

# Создаем STATIC библиотеку
add_library(GameTT_STATIC STATIC
    ${GAMETT_SOURCES}
    ${GAMETT_HEADERS}
)

# Создаем DLL
add_library(GameTT SHARED
    ${GAMETT_SOURCES}
    ${GAMETT_HEADERS}
    ${GAMETT_RC}
)

# Установка имени выходного файла
set_target_properties(GameTT_STATIC PROPERTIES
    OUTPUT_NAME "GameTT_static"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)

set_target_properties(GameTT PROPERTIES
    OUTPUT_NAME "GameTT"
    SUFFIX ".dll"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

# Зависимости GameTT из A7.dsw
target_link_libraries(GameTT_STATIC
    AILogic_STATIC
    BugSlay
    StreamIO_STATIC
    Common
    Formats
    RandomMapGen
    Misc
    # Системные библиотеки Windows
    version.lib
    comsupp.lib
)

target_link_libraries(GameTT
    AILogic
    BugSlay
    StreamIO
    Common
    Formats
    RandomMapGen
    Misc
    # Системные библиотеки Windows
    version.lib
    comsupp.lib
)

# Подключаем заголовочные файлы зависимостей
target_include_directories(GameTT_STATIC PRIVATE
    ${CMAKE_SOURCE_DIR}/AILogic
    ${CMAKE_SOURCE_DIR}/BugSlay
    ${CMAKE_SOURCE_DIR}/Misc
    ${CMAKE_SOURCE_DIR}/StreamIO
    ${CMAKE_SOURCE_DIR}/Common
    ${CMAKE_SOURCE_DIR}/Formats
    ${CMAKE_SOURCE_DIR}/RandomMapGen
)

target_include_directories(GameTT PRIVATE
    ${CMAKE_SOURCE_DIR}/AILogic
    ${CMAKE_SOURCE_DIR}/BugSlay
    ${CMAKE_SOURCE_DIR}/Misc
    ${CMAKE_SOURCE_DIR}/StreamIO
    ${CMAKE_SOURCE_DIR}/Common
    ${CMAKE_SOURCE_DIR}/Formats
    ${CMAKE_SOURCE_DIR}/RandomMapGen
)

# Базовые определения для Windows
target_compile_definitions(GameTT_STATIC PRIVATE
    WIN32
    _WINDOWS
    _LIB
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _WIN32_WINNT=0x0400
    WINVER=0x0400
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
)

target_compile_definitions(GameTT PRIVATE
    WIN32
    _WINDOWS
    _USRDLL
    GameTT_EXPORTS
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _WIN32_WINNT=0x0400
    WINVER=0x0400
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
)

# Отключаем проверку диапазонов и разрешаем wchar_t как unsigned short
target_compile_options(GameTT PRIVATE
    /Zc:wchar_t-
    /FS
)

target_compile_options(GameTT_STATIC PRIVATE
    /Zc:wchar_t-
    /FS
)

# Настройки прекомпилированных заголовков
set_target_properties(GameTT_STATIC PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
)

set_target_properties(GameTT PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
)

# Для StdAfx.cpp включаем создание PCH
set_source_files_properties(StdAfx.cpp PROPERTIES
    COMPILE_FLAGS "/YcStdAfx.h"
)
