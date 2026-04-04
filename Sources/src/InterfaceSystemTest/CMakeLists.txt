# InterfaceSystemTest - Тест интерфейса
# Зависит от: UI, BugSlay, Misc, StreamIO, LuaLib

cmake_minimum_required(VERSION 2.4)

project(InterfaceSystemTest)

FILE(GLOB SOURCES "*.cpp")
FILE(GLOB HEADERS "*.h")

add_executable(InterfaceSystemTest ${SOURCES} ${HEADERS})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../UI
    ${CMAKE_CURRENT_SOURCE_DIR}/../BugSlay
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/../StreamIO
    ${CMAKE_CURRENT_SOURCE_DIR}/../LuaLib
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(InterfaceSystemTest
    UI
    StreamIO
    LuaLib
    Misc
    BugSlay
    zlib
)
