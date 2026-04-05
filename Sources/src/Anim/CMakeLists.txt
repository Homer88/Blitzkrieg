# Anim - Анимация (DLL)
# Зависит от: BugSlay, StreamIO, Misc, Image, Formats

cmake_minimum_required(VERSION 3.5)

project(Anim LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Исходные файлы
FILE(GLOB ANIM_SOURCES "*.cpp")
FILE(GLOB ANIM_HEADERS "*.h")

add_library(Anim SHARED 
    ${ANIM_SOURCES} 
    ${ANIM_HEADERS}
    anim.rc
)

# Используем .def файл для экспорта
if(WIN32)
    set_target_properties(Anim PROPERTIES
        DEFINE_FILE "Animation.def"
        OUTPUT_NAME "Anim"
        PREFIX ""
    )
endif()

# Прекомпилированные заголовки
set_target_properties(Anim PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
)

set_source_files_properties(StdAfx.cpp PROPERTIES
    COMPILE_FLAGS "/YcStdAfx.h"
)

# Определения для Windows
target_compile_definitions(Anim PRIVATE
    WIN32
    _WINDOWS
    _USRDLL
    ANIM_EXPORTS
    _FINALRELEASE
    _DO_ASSERT_SLOW
    _DO_CHECKED_CAST
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
)

# Подключение заголовочных файлов зависимостей
target_include_directories(Anim PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/BugSlay
    ${CMAKE_SOURCE_DIR}/StreamIO
    ${CMAKE_SOURCE_DIR}/Misc
    ${CMAKE_SOURCE_DIR}/Image
    ${CMAKE_SOURCE_DIR}/Formats
)

# Подключение зависимостей
target_link_libraries(Anim PRIVATE
    BugSlay
    StreamIO_STATIC
    Misc
    Image
    Formats
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
