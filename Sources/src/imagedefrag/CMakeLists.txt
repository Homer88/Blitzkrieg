# ImageDefrag (ImagePacker) - Утилита дефрагментации/упаковки изображений
# На основе ImageDefrag.dsp (Visual Studio 6.0)
cmake_minimum_required(VERSION 3.5)

project(ImageDefrag LANGUAGES CXX)

# Стандарт C++98
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Определения из DSP файла
# Release: /O2 /W3 /GX /MD /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_DO_ASSERT_SLOW" /D "_DEVVERSION"
# Debug: /Od /W3 /Gm /GX /ZI /MDd /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_DO_ASSERT_SLOW"
add_compile_definitions(
    WIN32
    _CONSOLE
    _MBCS
    _DO_ASSERT_SLOW
    _DEVVERSION
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    _WIN32_WINNT=0x0501
    WINVER=0x0501
)

# Исходные файлы (из DSP + локальные .cpp)
set(SOURCES
    main.cpp
    GlobalsLoader.cpp
    StdAfx.cpp
)

set(HEADERS
    Specific.h
    StdAfx.h
)

# Консольное приложение
add_executable(ImageDefrag ${SOURCES} ${HEADERS})

# Precompiled headers
if(MSVC)
    set_source_files_properties(StdAfx.cpp PROPERTIES COMPILE_FLAGS "/Yc\"StdAfx.h\"")
    set_source_files_properties(main.cpp GlobalsLoader.cpp BetaSpline.cpp ImageEdge.cpp ImagePack.cpp PNoise.cpp TerrainGenerator.cpp PROPERTIES COMPILE_FLAGS "/Yu\"StdAfx.h\"")

    # Release
    target_compile_options(ImageDefrag PRIVATE $<$<CONFIG:Release>:/O2 /MD /GR>)
    # Debug
    target_compile_options(ImageDefrag PRIVATE $<$<CONFIG:Debug>:/Od /MDd /Gm /ZI /GZ /GR>)
endif()

# Включения
target_include_directories(ImageDefrag PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/../StreamIO
    ${CMAKE_CURRENT_SOURCE_DIR}/../BugSlay
    ${CMAKE_CURRENT_SOURCE_DIR}/../Image
    ${CMAKE_CURRENT_SOURCE_DIR}/../zlib
    ${CMAKE_CURRENT_SOURCE_DIR}/../libpng
)

# Библиотеки
target_link_libraries(ImageDefrag PRIVATE
    # Внутренние зависимости
    Image
    StreamIO_STATIC
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
set_target_properties(ImageDefrag PROPERTIES
    LINK_FLAGS "/subsystem:console /machine:I386"
)

if(MSVC)
    set_target_properties(ImageDefrag PROPERTIES
        LINK_FLAGS_DEBUG "/debug /pdbtype:sept"
    )
endif()
