# Game - Игровой модуль (Windows Application)
# Зависит от: Main, BugSlay, Misc, StreamIO, Common, Formats, AILogic,
#            Anim, Image, Input, Scene, SFX, GFX, UI, Net, GameSpy, LuaLib, RandomMapGen

cmake_minimum_required(VERSION 2.4)

project(Game)

# Определения компиляции (Debug конфигурация из Game.dsp)
add_definitions(
    -DWIN32
    -D_WINDOWS
    -D_DEBUG
    -D_DO_ASSERT_SLOW
    -D_DO_SEH
    -D_DO_CHECKED_CAST
    -D_STL_RANGE_CHECK
    -D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    -D_WIN32_WINNT=0x0501
    -D_CRT_SECURE_NO_WARNINGS
)

# Исходные файлы из Game.dsp
set(GAME_SOURCES
    # Common
    GlobalsLoader.cpp
    StdAfx.cpp

    # Main
    main.cpp

    # Frame
    SysKeys.cpp
    WinFrame.cpp
)

# Ресурсы
set(GAME_RESOURCES
    Game.rc
    main.ico
    resource.h
    splash.bmp
)

# Заголовки
FILE(GLOB GAME_HEADERS "*.h")

# Win32 Application
add_executable(Game WIN32 ${GAME_SOURCES} ${GAME_HEADERS} ${GAME_RESOURCES})

# Прекомпилированные заголовки
set_target_properties(Game PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
)

# wchar_t как unsigned short (совместимость с остальным проектом)
target_compile_options(Game PRIVATE /Zc:wchar_t-)

set_source_files_properties(StdAfx.cpp PROPERTIES
    COMPILE_FLAGS "/YcStdAfx.h"
)

# Include директории
target_include_directories(Game PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/BugSlay
    ${CMAKE_SOURCE_DIR}/Misc
    ${CMAKE_SOURCE_DIR}/StreamIO
    ${CMAKE_SOURCE_DIR}/Common
    ${CMAKE_SOURCE_DIR}/Formats
    ${CMAKE_SOURCE_DIR}/AILogic
    ${CMAKE_SOURCE_DIR}/Anim
    ${CMAKE_SOURCE_DIR}/Image
    ${CMAKE_SOURCE_DIR}/Input
    ${CMAKE_SOURCE_DIR}/Scene
    ${CMAKE_SOURCE_DIR}/SFX
    ${CMAKE_SOURCE_DIR}/GFX
    ${CMAKE_SOURCE_DIR}/UI
    ${CMAKE_SOURCE_DIR}/Net
    ${CMAKE_SOURCE_DIR}/GameSpy
    ${CMAKE_SOURCE_DIR}/LuaLib
    ${CMAKE_SOURCE_DIR}/RandomMapGen
    ${CMAKE_SOURCE_DIR}/Main
    # DirectX8 SDK
    ${CMAKE_SOURCE_DIR}/../sdk/directx/include
)

# Зависимости согласно Game.dsp (Debug: d3dx8.lib, version.lib, winmm.lib и системные)
target_link_libraries(Game
    # Основной модуль
    Main
    # Движок
    Scene
    AILogic
    Anim
    GFX
    UI
    SFX_STATIC
    Input
    Net
    GameSpy
    LuaLib
    RandomMapGen
    Common
    Formats
    Image
    StreamIO
    BugSlay
    Misc
    # DirectX8
    d3d8.lib
    d3dx8.lib
    dxguid.lib
    # Системные библиотеки из Game.dsp
    version.lib
    winmm.lib
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
    odbc32.lib
    odbccp32.lib
    comsupp.lib
    Shlwapi.lib
)
