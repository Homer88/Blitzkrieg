# Game - Игровой модуль
# Зависит от: BugSlay, Misc, StreamIO, Common, Formats, AILogic, 
#            Anim, Image, Input, Scene, SFX, GFX, UI, Net, GameSpy, LuaLib, RandomMapGen

cmake_minimum_required(VERSION 2.4)

project(Game)

FILE(GLOB SOURCES "*.cpp")
FILE(GLOB HEADERS "*.h")

add_executable(Game ${SOURCES} ${HEADERS})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../BugSlay
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/../StreamIO
    ${CMAKE_CURRENT_SOURCE_DIR}/../Common
    ${CMAKE_CURRENT_SOURCE_DIR}/../Formats
    ${CMAKE_CURRENT_SOURCE_DIR}/../AILogic
    ${CMAKE_CURRENT_SOURCE_DIR}/../Anim
    ${CMAKE_CURRENT_SOURCE_DIR}/../Image
    ${CMAKE_CURRENT_SOURCE_DIR}/../Input
    ${CMAKE_CURRENT_SOURCE_DIR}/../Scene
    ${CMAKE_CURRENT_SOURCE_DIR}/../SFX
    ${CMAKE_CURRENT_SOURCE_DIR}/../GFX
    ${CMAKE_CURRENT_SOURCE_DIR}/../UI
    ${CMAKE_CURRENT_SOURCE_DIR}/../Net
    ${CMAKE_CURRENT_SOURCE_DIR}/../GameSpy
    ${CMAKE_CURRENT_SOURCE_DIR}/../LuaLib
    ${CMAKE_CURRENT_SOURCE_DIR}/../RandomMapGen
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Зависимости будут настроены после сборки всех библиотек
# target_link_libraries(Game ...)
