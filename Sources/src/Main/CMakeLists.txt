# Main - Основной модуль игры (Static Library)
# Зависимости: BugSlay, Misc, StreamIO, Common, Formats, RandomMapGen, AILogic, GameTT, UI, Input, Anim, GFX, SFX, scl, Net, Image, LuaLib, GameSpy, Scene

cmake_minimum_required(VERSION 2.4)
project(Main)

# Установка стандарта C++98
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Определения компиляции из DSP (Debug конфигурация)
add_definitions(
    -DWIN32
    -D_WINDOWS
    -D_DEBUG
    -D_DO_ASSERT_SLOW
    -D_DO_CHECKED_CAST
    -D_STL_RANGE_CHECK
    -D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    -D_WIN32_WINNT=0x0501
    -D_CRT_SECURE_NO_WARNINGS
)

# Собираем все исходные файлы из DSP
set(MAIN_SOURCES
    # Common
    StdAfx.cpp

    # Interface
    iMainInternal.cpp
    MainLoopCommands.cpp
    MainObjectFactory.cpp
    RandomMapHelper.cpp

    # Timer
    GameTimerInternal.cpp

    # GDB
    GameDB.cpp
    GameStats.cpp
    RPGStats.cpp

    # Other
    FilesInspector.cpp
    InitGlobalVarConsts.cpp
    Initialization.cpp
    LoadDLLs.cpp

    # Text
    TextManager.cpp
    TextObject.cpp

    # Transceiver
    AILogicCommand.cpp
    AILogicCommandInternal.cpp
    MultiPlayerTransceiver.cpp
    SinglePlayerTransceiver.cpp

    # Multiplayer - Messages
    ChatMessages.cpp
    GameCreationMessages.cpp
    MessagesStore.cpp
    ServersListMessages.cpp

    # Multiplayer
    assert.cpp
    GameCreation.cpp
    GamePlaying.cpp
    GameSpyChat.cpp
    GameSpyPeerChat.cpp
    LanChat.cpp
    MultiplayerInternal.cpp
    ServerInfo.cpp
    ServersList.cpp

    # CommandsHistory
    CommandsHistory.cpp

    # Scenario Tracker
    PlayerScenarioInfo.cpp
    PlayerSkill.cpp
    ScenarioStatistics.cpp
    ScenarioTracker2Internal.cpp
    UserProfile.cpp

    # Beta Protection
    BetaKey.cpp
)

# Все заголовки в директории Main
FILE(GLOB MAIN_HEADERS "*.h")

# Создаём статическую библиотеку
add_library(Main STATIC ${MAIN_SOURCES} ${MAIN_HEADERS})

# Прекомпилированные заголовки
set_target_properties(Main PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
)
set_source_files_properties(StdAfx.cpp PROPERTIES
    COMPILE_FLAGS "/YcStdAfx.h"
)

# wchar_t как unsigned short (совместимость с остальным проектом)
target_compile_options(Main PRIVATE /Zc:wchar_t-)

# Include директории - зависимости из Main.dsp и структуры проекта
target_include_directories(Main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/AILogic
    ${CMAKE_SOURCE_DIR}/BugSlay
    ${CMAKE_SOURCE_DIR}/Misc
    ${CMAKE_SOURCE_DIR}/StreamIO
    ${CMAKE_SOURCE_DIR}/Common
    ${CMAKE_SOURCE_DIR}/Formats
    ${CMAKE_SOURCE_DIR}/RandomMapGen
    ${CMAKE_SOURCE_DIR}/GameTT
    ${CMAKE_SOURCE_DIR}/UI
    ${CMAKE_SOURCE_DIR}/Input
    ${CMAKE_SOURCE_DIR}/Anim
    ${CMAKE_SOURCE_DIR}/GFX
    ${CMAKE_SOURCE_DIR}/SFX
    ${CMAKE_SOURCE_DIR}/scl
    ${CMAKE_SOURCE_DIR}/Net
    ${CMAKE_SOURCE_DIR}/Image
    ${CMAKE_SOURCE_DIR}/LuaLib
    ${CMAKE_SOURCE_DIR}/GameSpy
    ${CMAKE_SOURCE_DIR}/Scene
)

# Зависимости из A7.dsw
target_link_libraries(Main
    AILogic
    BugSlay
    StreamIO
    Common
    Formats
    RandomMapGen
    GameTT
    UI
    Input
    Anim
    GFX
    SFX
    scl
    Net
    Image
    LuaLib
    GameSpy
    Scene
    Misc
    # Системные библиотеки
    comsupp.lib
    winmm.lib
    ws2_32.lib
    version.lib
)
