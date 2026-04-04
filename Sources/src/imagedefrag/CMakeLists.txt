# imagedefrag - Дефрагментация изображений
# Зависит от: BugSlay, Image, Misc, StreamIO, RandomMapGen, Formats, Main

cmake_minimum_required(VERSION 2.4)

project(ImageDefrag)

FILE(GLOB SOURCES "*.cpp")
FILE(GLOB HEADERS "*.h")

add_executable(ImageDefrag ${SOURCES} ${HEADERS})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../BugSlay
    ${CMAKE_CURRENT_SOURCE_DIR}/../Image
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/../StreamIO
    ${CMAKE_CURRENT_SOURCE_DIR}/../RandomMapGen
    ${CMAKE_CURRENT_SOURCE_DIR}/../Formats
    ${CMAKE_CURRENT_SOURCE_DIR}/../Main
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(ImageDefrag
    Image
    StreamIO
    RandomMapGen
    Formats
    Main
    Misc
    BugSlay
    libpng
    zlib
)
