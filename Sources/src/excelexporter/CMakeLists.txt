# ExcelExporter - Консольное приложение для экспорта/импорта в Excel
# На основе ExcelExporter.dsp (Visual Studio 6.0)
cmake_minimum_required(VERSION 3.5)

# Поддержка MFC (динамическая)
set(CMAKE_MFC_FLAG 2)

project(ExcelExporter LANGUAGES CXX)

# Стандарт C++98
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Определения из DSP файла
add_compile_definitions(
    WIN32
    _CONSOLE
    _MBCS
    __AFX__
    _AFXDLL
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    _WIN32_WINNT=0x0501
    WINVER=0x0501
)

# Исходные файлы (из DSP, XmlReader переписан на tinyxml2 вместо MSXML COM)
set(SOURCES
    GlobalsLoader.cpp
    StdAfx.cpp
    ExcelExporter.cpp
    main.cpp
    XmlReader.cpp
)

set(HEADERS
    Specific.h
    StdAfx.h
    ExcelExporter.h
    XmlReader.h
)

# Консольное приложение
add_executable(ExcelExporter ${SOURCES} ${HEADERS})

# Precompiled headers
if(MSVC)
    set_source_files_properties(StdAfx.cpp PROPERTIES COMPILE_FLAGS "/Yc\"StdAfx.h\"")
    set_source_files_properties(GlobalsLoader.cpp ExcelExporter.cpp main.cpp XmlReader.cpp PROPERTIES COMPILE_FLAGS "/Yu\"StdAfx.h\"")

    # Release
    target_compile_options(ExcelExporter PRIVATE $<$<CONFIG:Release>:/O2 /MD>)
    # Debug
    target_compile_options(ExcelExporter PRIVATE $<$<CONFIG:Debug>:/Od /MDd /Gm /ZI /GZ>)
endif()

# Включения
target_include_directories(ExcelExporter PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/../StreamIO
    ${CMAKE_CURRENT_SOURCE_DIR}/../BugSlay
    ${CMAKE_CURRENT_SOURCE_DIR}/../Main
    ${CMAKE_CURRENT_SOURCE_DIR}/../zlib
)

# Библиотеки
target_link_libraries(ExcelExporter PRIVATE
    # Внутренние зависимости
    StreamIO_STATIC
    Misc
    BugSlay
    zlib
    tinyxml2
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
    comsuppw
    winmm
    odbc32
    legacy_stdio_definitions
)

# Подсистема
set_target_properties(ExcelExporter PROPERTIES
    LINK_FLAGS "/subsystem:console /machine:I386"
)

if(MSVC)
    set_target_properties(ExcelExporter PROPERTIES
        LINK_FLAGS_DEBUG "/debug /pdbtype:sept"
    )
endif()
