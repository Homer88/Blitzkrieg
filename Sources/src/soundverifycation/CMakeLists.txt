# SoundVerifycation - Утилита проверки звука
# Консольное приложение с использованием FMOD
# Зависимости: BugSlay, Misc, StreamIO, FMOD

cmake_minimum_required(VERSION 3.5)

project(SoundVerifycation LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Исходные файлы
FILE(GLOB SOURCES "*.cpp")
FILE(GLOB HEADERS "*.h")

add_executable(SoundVerifycation ${SOURCES} ${HEADERS})

# Прекомпилированные заголовки
set_target_properties(SoundVerifycation PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
)

set_source_files_properties(StdAfx.cpp PROPERTIES
    COMPILE_FLAGS "/YcStdAfx.h"
)

# Определения для Windows
target_compile_definitions(SoundVerifycation PRIVATE
    WIN32
    _CONSOLE
    _MBCS
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    _WIN32_WINNT=0x0400
    WINVER=0x0400
)

# FMOD SDK
set(FMOD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../sdk/fmod/api/lowlevel/inc")
set(FMOD_LIB_DIR "${CMAKE_SOURCE_DIR}/../sdk/fmod/api/lowlevel/lib")

if(EXISTS "${FMOD_INCLUDE_DIR}/fmod.hpp")
    message(STATUS "FMOD SDK found for SoundVerifycation: ${FMOD_INCLUDE_DIR}")
    set(FMOD_FOUND TRUE)
else()
    message(WARNING "FMOD SDK NOT found! SoundVerifycation will not link correctly.")
    set(FMOD_FOUND FALSE)
endif()

# Подключение заголовочных файлов
target_include_directories(SoundVerifycation PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/BugSlay
    ${CMAKE_SOURCE_DIR}/Misc
    ${CMAKE_SOURCE_DIR}/StreamIO
    ${FMOD_INCLUDE_DIR}
)

# Подключение зависимостей
target_link_libraries(SoundVerifycation PRIVATE
    BugSlay
    Misc
    StreamIO_STATIC
)

# Подключение FMOD
if(FMOD_FOUND AND EXISTS "${FMOD_LIB_DIR}")
    target_link_directories(SoundVerifycation PRIVATE ${FMOD_LIB_DIR})
    
    # Ищем FMOD библиотеку
    if(EXISTS "${FMOD_LIB_DIR}/fmod_vc.lib")
        target_link_libraries(SoundVerifycation PRIVATE fmod_vc.lib)
        message(STATUS "Using FMOD library: fmod_vc.lib for SoundVerifycation")
    elseif(EXISTS "${FMOD_LIB_DIR}/fmodL_vc.lib")
        target_link_libraries(SoundVerifycation PRIVATE fmodL_vc.lib)
        message(STATUS "Using FMOD library: fmodL_vc.lib for SoundVerifycation")
    elseif(EXISTS "${FMOD_LIB_DIR}/fmod.lib")
        target_link_libraries(SoundVerifycation PRIVATE fmod.lib)
        message(STATUS "Using FMOD library: fmod.lib for SoundVerifycation")
    elseif(EXISTS "${FMOD_LIB_DIR}/fmodvc.lib")
        target_link_libraries(SoundVerifycation PRIVATE fmodvc.lib)
        message(STATUS "Using FMOD library: fmodvc.lib for SoundVerifycation")
    endif()
endif()

# Системные библиотеки Windows
target_link_libraries(SoundVerifycation PRIVATE
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
    winmm.lib
)
