# bzmconvertor - Конвертор BZM файлов
# Зависит от: Formats, Misc, RandomMapGen, StreamIO, Main

cmake_minimum_required(VERSION 2.4)

project(BZMConvertor)

FILE(GLOB SOURCES "*.cpp")
FILE(GLOB HEADERS "*.h")

add_executable(BZMConvertor ${SOURCES} ${HEADERS})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../Formats
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/../RandomMapGen
    ${CMAKE_CURRENT_SOURCE_DIR}/../StreamIO
    ${CMAKE_CURRENT_SOURCE_DIR}/../Main
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(BZMConvertor
    Main
    StreamIO
    RandomMapGen
    Formats
    Misc
    BugSlay
    zlib
)
