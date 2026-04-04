# FontGen - Генератор шрифтов
# Зависит от: BugSlay, Image, StreamIO, Misc, Common, Formats

cmake_minimum_required(VERSION 2.4)

project(FontGen)

FILE(GLOB SOURCES "*.cpp")
FILE(GLOB HEADERS "*.h")

add_executable(FontGen ${SOURCES} ${HEADERS})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../BugSlay
    ${CMAKE_CURRENT_SOURCE_DIR}/../Image
    ${CMAKE_CURRENT_SOURCE_DIR}/../StreamIO
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/../Common
    ${CMAKE_CURRENT_SOURCE_DIR}/../Formats
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(FontGen
    Image
    StreamIO
    Common
    Formats
    Misc
    BugSlay
    libpng
    zlib
)
