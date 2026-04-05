# Image - Библиотека работы с изображениями
# Зависит от: libpng, zlib, BugSlay, Misc

cmake_minimum_required(VERSION 2.4)

project(Image)

FILE(GLOB SOURCES "*.cpp")
FILE(GLOB HEADERS "*.h")

add_library(Image STATIC ${SOURCES} ${HEADERS})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../libpng
    ${CMAKE_CURRENT_SOURCE_DIR}/../zlib
    ${CMAKE_CURRENT_SOURCE_DIR}/../BugSlay
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/../sdk/S3TC
)

target_link_libraries(Image
    LibPng
    zlib
    BugSlay
    Misc
)
