# Input - Библиотека ввода
# Зависит от: BugSlay, Misc, StreamIO

cmake_minimum_required(VERSION 2.4)

project(Input)

FILE(GLOB SOURCES "*.cpp")
FILE(GLOB HEADERS "*.h")

add_library(Input STATIC ${SOURCES} ${HEADERS})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../BugSlay
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/../StreamIO
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(Input
    StreamIO
    Misc
    BugSlay
    zlib
)
