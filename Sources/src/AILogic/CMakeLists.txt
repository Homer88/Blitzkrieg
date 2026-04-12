# AILogic - ИИ логика игры (DLL + STATIC)
# Совместимость: MSVC 2002-2022, C++98 стандарт
# Зависимости: BugSlay, StreamIO, Misc, Image, LuaLib, Formats, zlib, RandomMapGen

cmake_minimum_required(VERSION 3.0)

project(AILogic LANGUAGES CXX)

# Установка стандарта C++98
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Исходные файлы
FILE(GLOB AILOGIC_SOURCE "*.cpp" "Scripts/*.cpp")
FILE(GLOB AILOGIC_HEADERS "*.h" "Scripts/*.h")

# Создаем STATIC библиотеку
add_library(AILogic_STATIC STATIC
    ${AILOGIC_SOURCE}
    ${AILOGIC_HEADERS}
)

# Создаем DLL
add_library(AILogic SHARED
    ${AILOGIC_SOURCE}
    ${AILOGIC_HEADERS}
    AILogic.rc
    AILogic.def
)

# Установка имени выходного файла и создание import library
set_target_properties(AILogic_STATIC PROPERTIES
    OUTPUT_NAME "AILogic_static"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)

set_target_properties(AILogic PROPERTIES
    OUTPUT_NAME "AILogic"
    PREFIX ""
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

# Создаем import library вручную если его нет
# На Windows DLL + .lib для импорта создаются автоматически при экспорте символов
if(WIN32)
    set_target_properties(AILogic PROPERTIES
        DEFINE_FILE "AILogic.def"
        WINDOWS_EXPORT_ALL_SYMBOLS FALSE
    )
endif()

# Прекомпилированные заголовки для STATIC
set_target_properties(AILogic_STATIC PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h /Zm200"
)

set_source_files_properties(StdAfx.cpp PROPERTIES
    COMPILE_FLAGS "/YcStdAfx.h"
)

# Прекомпилированные заголовки для DLL
set_target_properties(AILogic PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h /Zm200"
)

# Определения для Windows (согласно .dsp)
target_compile_definitions(AILogic_STATIC PRIVATE
    WIN32
    _WINDOWS
    _LIB
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    _WIN32_WINNT=0x0400
    WINVER=0x0400
)

target_compile_definitions(AILogic PRIVATE
    WIN32
    _WINDOWS
    _USRDLL
    AILogic_EXPORTS
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    _WIN32_WINNT=0x0400
    WINVER=0x0400
)

# Отключаем некоторые ошибки для MSVC 2022
# C2908 - явная специализация уже создана (дубликаты CastToRefCountImpl)
# C4430 - отсутствует спецификатор типа
target_compile_options(AILogic PRIVATE
    /wd4430
)

target_compile_options(AILogic_STATIC PRIVATE
    /wd4430
)

# Для файлов с BASIC_REGISTER_CLASS отключаем PCH чтобы избежать дубликатов
set_source_files_properties(
    Aviation.cpp
    Formation.cpp
    PathFinderInternal.cpp
    ShootEstimatorInternal.cpp
    StandartPath.cpp
    Statistics.cpp
    UnitStates.cpp
    PROPERTIES COMPILE_FLAGS "/Y-"
)

# Подключение заголовочных файлов
target_include_directories(AILogic_STATIC PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/BugSlay
    ${CMAKE_SOURCE_DIR}/Misc
    ${CMAKE_SOURCE_DIR}/StreamIO
    ${CMAKE_SOURCE_DIR}/Image
    ${CMAKE_SOURCE_DIR}/LuaLib
    ${CMAKE_SOURCE_DIR}/Formats
    ${CMAKE_SOURCE_DIR}/zlib
    ${CMAKE_SOURCE_DIR}/RandomMapGen
)

target_include_directories(AILogic PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/BugSlay
    ${CMAKE_SOURCE_DIR}/Misc
    ${CMAKE_SOURCE_DIR}/StreamIO
    ${CMAKE_SOURCE_DIR}/Image
    ${CMAKE_SOURCE_DIR}/LuaLib
    ${CMAKE_SOURCE_DIR}/Formats
    ${CMAKE_SOURCE_DIR}/zlib
    ${CMAKE_SOURCE_DIR}/RandomMapGen
)

# Подключение зависимостей (согласно AILogic.dsp)
target_link_libraries(AILogic_STATIC PRIVATE
    BugSlay
    StreamIO_STATIC
    Misc
    Image
    LuaLib
    Formats
    zlib
    RandomMapGen
    # Системные библиотеки Windows
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

target_link_libraries(AILogic PRIVATE
    BugSlay
    StreamIO
    Misc
    Image
    LuaLib
    Formats
    zlib
    RandomMapGen
    # Системные библиотеки Windows
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
