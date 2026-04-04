# ELK - ELK библиотека
# Зависит от: Misc, RandomMapGen, zlib, StreamIO, BugSlay, Image, Formats

cmake_minimum_required(VERSION 2.4)

project(ELK)

FILE(GLOB SOURCES "*.cpp")
FILE(GLOB HEADERS "*.h")

add_library(ELK STATIC ${SOURCES} ${HEADERS})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/../RandomMapGen
    ${CMAKE_CURRENT_SOURCE_DIR}/../zlib
    ${CMAKE_CURRENT_SOURCE_DIR}/../StreamIO
    ${CMAKE_CURRENT_SOURCE_DIR}/../BugSlay
    ${CMAKE_CURRENT_SOURCE_DIR}/../Image
    ${CMAKE_CURRENT_SOURCE_DIR}/../Formats
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(ELK
    RandomMapGen
    StreamIO
    Image
    Formats
    Misc
    BugSlay
    libpng
    zlib
)
