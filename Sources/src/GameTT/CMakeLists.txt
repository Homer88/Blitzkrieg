# ============================================================
# GameTT - Основной игровой модуль
# Зависимости: BugSlay, Misc, StreamIO, Common, Formats, RandomMapGen
# ============================================================

# Собираем все исходные файлы
FILE(GLOB GAMETT_SOURCES "*.cpp")
FILE(GLOB GAMETT_HEADERS "*.h")

# Resource file
set(GAMETT_RC "GameTT.rc")

# Определяем имя библиотеки
add_library(GameTT SHARED
    ${GAMETT_SOURCES}
    ${GAMETT_HEADERS}
    ${GAMETT_RC}
)

# Зависимости GameTT из A7.dsw
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
target_compile_definitions(GameTT PRIVATE
    WIN32
    _WINDOWS
    _USRDLL
    GameTT_EXPORTS
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    _WIN32_WINNT=0x0400
    WINVER=0x0400
)

# Отключаем проверку диапазонов и разрешаем wchar_t как unsigned short
target_compile_options(GameTT PRIVATE
    /Zc:wchar_t-
)

# Настройки прекомпилированных заголовков
set_target_properties(GameTT PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
)

# Для StdAfx.cpp включаем создание PCH
set_source_files_properties(StdAfx.cpp PROPERTIES
    COMPILE_FLAGS "/YcStdAfx.h"
)

# Установка имени выходного файла
set_target_properties(GameTT PROPERTIES
    OUTPUT_NAME "GameTT"
    SUFFIX ".dll"
)
