# BetaKeyGen - Консольное приложение для генерации бета-ключей
cmake_minimum_required(VERSION 3.5)
project(BetaKeyGen LANGUAGES C CXX)

# Стандарт C++98 для совместимости с Windows 98/2000
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Определение целевой платформы Windows 98/2000
add_definitions(
    -DWIN32
    -D_CONSOLE
    -D_MBCS
    -D_DO_BETA_CHECK
    -D_DEVVERSION
    -D_WIN32_WINNT=0x0400
    -DNOMINMAX
    -D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
)

# Исходные файлы проекта (из BetaKeyGen.dsp)
set(SOURCES
    BetaKey.cpp
    main.cpp
    StdAfx.cpp
    ../Misc/StrProc.cpp
    ../Misc/FileUtils.cpp
    ../zlib/adler32.c
    ../zlib/compress.c
    ../zlib/crc32.c
    ../zlib/deflate.c
    ../zlib/gzio.c
    ../zlib/infblock.c
    ../zlib/infcodes.c
    ../zlib/inffast.c
    ../zlib/inflate.c
    ../zlib/inftrees.c
    ../zlib/infutil.c
    ../zlib/trees.c
    ../zlib/uncompr.c
    ../zlib/zutil.c
)

# Заголовочные файлы
set(HEADERS
    Specific.h
    StdAfx.h
)

# Создаём консольное приложение
add_executable(BetaKeyGen ${SOURCES} ${HEADERS})

# Настройки компилятора из DSP файла:
# Release: /O2 /W3 /GX /MD /YX /FD /c
# Debug: /Od /W3 /Gm /GR /GX /ZI /MDd /YX /FD /GZ /c
if(MSVC)
    # Отключаем предупреждения
    target_compile_options(BetaKeyGen PRIVATE /W3 /GR)
    
    # Precompiled header для StdAfx.cpp
    set_source_files_properties(StdAfx.cpp PROPERTIES COMPILE_FLAGS "/Yc\"StdAfx.h\"")
    
    # Использование precompiled header для остальных файлов
    set_source_files_properties(BetaKey.cpp main.cpp PROPERTIES COMPILE_FLAGS "/Yu\"StdAfx.h\"")
    
    # Release конфигурация
    target_compile_options(BetaKeyGen PRIVATE $<$<CONFIG:Release>:/O2 /MD>)
    
    # Debug конфигурация
    target_compile_options(BetaKeyGen PRIVATE $<$<CONFIG:Debug>:/Od /MDd /Gm /ZI /GZ>)
endif()

# Библиотеки из DSP файла
target_link_libraries(BetaKeyGen PRIVATE
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
    odbc32
    odbccp32
)

# Подсистема - консольное приложение, машина I386
set_target_properties(BetaKeyGen PROPERTIES
    LINK_FLAGS "/subsystem:console /machine:I386"
)

# Файлы zlib компилируются как C
set_source_files_properties(
    ../zlib/adler32.c
    ../zlib/compress.c
    ../zlib/crc32.c
    ../zlib/deflate.c
    ../zlib/gzio.c
    ../zlib/infblock.c
    ../zlib/infcodes.c
    ../zlib/inffast.c
    ../zlib/inflate.c
    ../zlib/inftrees.c
    ../zlib/infutil.c
    ../zlib/trees.c
    ../zlib/uncompr.c
    ../zlib/zutil.c
    PROPERTIES LANGUAGE C
)

# Debug линковка
if(MSVC)
    set_target_properties(BetaKeyGen PROPERTIES
        LINK_FLAGS_DEBUG "/debug /pdbtype:sept"
    )
endif()

# Пути включения (зависимости от других модулей проекта)
target_include_directories(BetaKeyGen PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/../zlib
    ${CMAKE_CURRENT_SOURCE_DIR}/../BugSlay
    ${CMAKE_CURRENT_SOURCE_DIR}/../StreamIO
    ${CMAKE_CURRENT_SOURCE_DIR}/../Main
)
