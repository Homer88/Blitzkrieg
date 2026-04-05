# Минимальная версия CMake
cmake_minimum_required(VERSION 3.5)

# Название проекта
project(SFX LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB SFX_SOURCE "*.cpp")
FILE(GLOB SFX_HEADERS "*.h")

# FMOD SDK
set(FMOD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../sdk/fmod/api/lowlevel/inc")
set(FMOD_LIB_DIR "${CMAKE_SOURCE_DIR}/../sdk/fmod/api/lowlevel/lib")

# Проверяем существование FMOD
if(EXISTS "${FMOD_INCLUDE_DIR}/fmod.hpp")
    message(STATUS "FMOD SDK found: ${FMOD_INCLUDE_DIR}")
    set(FMOD_FOUND TRUE)
else()
    message(WARNING "FMOD SDK NOT found at ${FMOD_INCLUDE_DIR}")
    message(WARNING "SFX will compile but will NOT link correctly without FMOD libraries.")
    set(FMOD_FOUND FALSE)
endif()

# Определение SFX_DLL при сборке в DLL
option(SFX_BUILD_DLL "Build SFX as DLL" OFF)

if(SFX_BUILD_DLL)
    add_library(SFX SHARED ${SFX_SOURCE} ${SFX_HEADERS} SFX.rc)
    
    # Используем .def файл для экспорта функций
    if(WIN32)
        set_target_properties(SFX PROPERTIES
            DEFINE_FILE "Sound.def"
            OUTPUT_NAME "SFX"
            PREFIX ""
        )
    endif()
    
    target_compile_definitions(SFX PRIVATE
        SFX_EXPORTS
        _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
        _WIN32_WINNT=0x0501
        WIN32
        _WINDOWS
        _USRDLL
    )
    
    # Добавляем пути для заголовков
    target_include_directories(SFX PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_SOURCE_DIR}/../sdk/fmod/api/lowlevel/inc
        ${CMAKE_SOURCE_DIR}/StreamIO
        ${CMAKE_SOURCE_DIR}/Misc
    )
    
    # Подключение зависимостей и FMOD
    target_link_libraries(SFX PRIVATE
        BugSlay
        Misc
        StreamIO_STATIC
        winmm.lib
    )
    
    if(FMOD_FOUND)
        if(EXISTS "${FMOD_LIB_DIR}")
            target_link_directories(SFX PRIVATE ${FMOD_LIB_DIR})
            # Ищем FMOD библиотеку (новые имена: fmod_vc.lib, fmodL_vc.lib)
            if(EXISTS "${FMOD_LIB_DIR}/fmod_vc.lib")
                target_link_libraries(SFX PRIVATE fmod_vc.lib)
                message(STATUS "Using FMOD library: fmod_vc.lib")
            elseif(EXISTS "${FMOD_LIB_DIR}/fmodL_vc.lib")
                target_link_libraries(SFX PRIVATE fmodL_vc.lib)
                message(STATUS "Using FMOD library: fmodL_vc.lib")
            elseif(EXISTS "${FMOD_LIB_DIR}/fmod.lib")
                target_link_libraries(SFX PRIVATE fmod.lib)
                message(STATUS "Using FMOD library: fmod.lib")
            elseif(EXISTS "${FMOD_LIB_DIR}/fmodvc.lib")
                target_link_libraries(SFX PRIVATE fmodvc.lib)
                message(STATUS "Using FMOD library: fmodvc.lib")
            else()
                message(WARNING "FMOD include found but no .lib files in ${FMOD_LIB_DIR}")
                message(WARNING "You need to provide FMOD libraries for SFX.dll to link.")
            endif()
        else()
            message(WARNING "FMOD lib directory not found at ${FMOD_LIB_DIR}")
            message(WARNING "You need to provide FMOD libraries for SFX.dll to link.")
        endif()
    endif()
else()
    add_library(SFX_STATIC STATIC ${SFX_SOURCE} ${SFX_HEADERS} SFX.rc)
    
    target_compile_definitions(SFX_STATIC PRIVATE
        _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
        _WIN32_WINNT=0x0501
        WIN32
        _WINDOWS
        _LIB
    )
    
    # Прекомпилированные заголовки
    set_target_properties(SFX_STATIC PROPERTIES
        COMPILE_FLAGS "/YuStdAfx.h"
    )
    set_source_files_properties(StdAfx.cpp PROPERTIES
        COMPILE_FLAGS "/YcStdAfx.h"
    )
    
    # Добавляем пути для заголовков
    target_include_directories(SFX_STATIC PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_SOURCE_DIR}/../sdk/fmod/api/lowlevel/inc
        ${CMAKE_SOURCE_DIR}/StreamIO
        ${CMAKE_SOURCE_DIR}/Misc
    )
    
    # Подключение зависимостей
    target_link_libraries(SFX_STATIC PRIVATE
        BugSlay
        Misc
        StreamIO_STATIC
        winmm.lib
    )
endif()

# Прекомпилированные заголовки для DLL
if(SFX_BUILD_DLL)
    set_target_properties(SFX PROPERTIES
        COMPILE_FLAGS "/YuStdAfx.h"
    )
    set_source_files_properties(StdAfx.cpp PROPERTIES
        COMPILE_FLAGS "/YcStdAfx.h"
    )
endif()
