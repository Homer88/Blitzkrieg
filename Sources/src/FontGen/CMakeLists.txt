# FontGen - Генератор шрифтов (консольное приложение)
# На основе FontGen.dsp (Visual Studio 6.0)
cmake_minimum_required(VERSION 3.5)

project(FontGen LANGUAGES CXX)

# Стандарт C++98
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Определения из DSP файла
# Release: /O2 /W3 /GX /MD /D "NDEBUG" /D "_CONSOLE" /D "_UNICODE" /D "_DO_ASSERT_SLOW"
# Debug: /Od /W3 /Gm /GX /ZI /MDd /D "_DEBUG" /D "_CONSOLE" /D "_UNICODE" /D "_DO_ASSERT_SLOW"
# ReleaseDebug: /O2 /W3 /GX /Zi /MD /D "NDEBUG" /D "_CONSOLE" /D "_UNICODE" /D "_DO_ASSERT_SLOW"
# BetaRelease: /O2 /W3 /GX /Zd /MD /D "NDEBUG" /D "_CONSOLE" /D "_UNICODE" /D "_DO_ASSERT_SLOW"
# FastDebug: /Od /W3 /Gm /GX /ZI /MDd /D "_DEBUG" /D "_CONSOLE" /D "_UNICODE" /D "_DO_ASSERT_SLOW" /D "_STL_FAST_DEBUG"
add_compile_definitions(
    WIN32
    _CONSOLE
    _MBCS
    _DO_ASSERT_SLOW
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    _WIN32_WINNT=0x0501
    WINVER=0x0501
)

# Исходные файлы (из DSP)
set(SOURCES
    GlobalsLoader.cpp
    StdAfx.cpp
    FontGen.cpp
)

set(HEADERS
    Specific.h
    StdAfx.h
    FontGen.h
)

# Консольное приложение
add_executable(FontGen ${SOURCES} ${HEADERS})

# Precompiled headers
if(MSVC)
    set_source_files_properties(StdAfx.cpp PROPERTIES COMPILE_FLAGS "/Yc\"StdAfx.h\"")
    set_source_files_properties(GlobalsLoader.cpp FontGen.cpp PROPERTIES COMPILE_FLAGS "/Yu\"StdAfx.h\"")

    # Release
    target_compile_options(FontGen PRIVATE $<$<CONFIG:Release>:/O2 /MD>)
    # Debug
    target_compile_options(FontGen PRIVATE $<$<CONFIG:Debug>:/Od /MDd /Gm /ZI /GZ>)
    # ReleaseDebug
    target_compile_options(FontGen PRIVATE $<$<CONFIG:ReleaseDebug>:/O2 /MD /Zi>)
    # BetaRelease
    target_compile_options(FontGen PRIVATE $<$<CONFIG:BetaRelease>:/O2 /MD /Zd>)
    # FastDebug
    target_compile_options(FontGen PRIVATE $<$<CONFIG:FastDebug>:/Od /MDd /Gm /ZI /GZ>)
endif()

# Включения
target_include_directories(FontGen PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/../StreamIO
    ${CMAKE_CURRENT_SOURCE_DIR}/../BugSlay
    ${CMAKE_CURRENT_SOURCE_DIR}/../Image
    ${CMAKE_CURRENT_SOURCE_DIR}/../Common
    ${CMAKE_CURRENT_SOURCE_DIR}/../Formats
    ${CMAKE_CURRENT_SOURCE_DIR}/../zlib
    ${CMAKE_CURRENT_SOURCE_DIR}/../libpng
    ${CMAKE_CURRENT_SOURCE_DIR}/../Main
)

# Библиотеки
target_link_libraries(FontGen PRIVATE
    # Внутренние зависимости
    Image
    StreamIO_STATIC
    Common
    Formats
    Misc
    BugSlay
    LibPng
    zlib
    # Windows системные
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
)

# Подсистема
set_target_properties(FontGen PROPERTIES
    LINK_FLAGS "/subsystem:console /machine:I386"
)

if(MSVC)
    set_target_properties(FontGen PROPERTIES
        LINK_FLAGS_DEBUG "/debug /pdbtype:sept"
    )
endif()
