# UI - Пользовательский интерфейс (DLL)
# Зависит от: BugSlay, Misc, StreamIO, LuaLib

cmake_minimum_required(VERSION 3.5)

project(UI LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Исходные файлы
FILE(GLOB UI_SOURCE "*.cpp")
FILE(GLOB UI_HEADERS "*.h")

add_library(UI SHARED ${UI_SOURCE} ${UI_HEADERS} UI.rc)

# Используем .def файл для экспорта
if(WIN32)
    target_link_options(UI PRIVATE "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/UI.def")
endif()

# Прекомпилированные заголовки
set_target_properties(UI PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
)

# wchar_t как unsigned short (совместимость с остальным проектом)
target_compile_options(UI PRIVATE /Zc:wchar_t-)

set_source_files_properties(StdAfx.cpp PROPERTIES
    COMPILE_FLAGS "/YcStdAfx.h"
)

# Определения для Windows
target_compile_definitions(UI PRIVATE
    WIN32
    _WINDOWS
    _USRDLL
    UI_EXPORTS
    _FINALRELEASE
    _DO_ASSERT_SLOW
    _DO_CHECKED_CAST
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    _WIN32_WINNT=0x0501
)

# Подключение заголовочных файлов зависимостей
target_include_directories(UI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/BugSlay
    ${CMAKE_SOURCE_DIR}/Misc
    ${CMAKE_SOURCE_DIR}/StreamIO
    ${CMAKE_SOURCE_DIR}/LuaLib
)

# Подключение зависимостей
target_link_libraries(UI PRIVATE
    BugSlay
    StreamIO_STATIC
    Misc
    LuaLib
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
