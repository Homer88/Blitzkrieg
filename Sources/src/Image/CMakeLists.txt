# Image - Библиотека работы с изображениями (DLL + STATIC)
# Зависит от: libpng, zlib, BugSlay, Misc

cmake_minimum_required(VERSION 2.4)

project(Image)

FILE(GLOB SOURCES "*.cpp")
FILE(GLOB HEADERS "*.h")

# Создаем STATIC библиотеку
add_library(Image STATIC ${SOURCES} ${HEADERS})

# Создаем DLL
add_library(Image_DLL SHARED ${SOURCES} ${HEADERS})

# Установка имени выходного файла
set_target_properties(Image PROPERTIES
    OUTPUT_NAME "Image_static"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)

set_target_properties(Image_DLL PROPERTIES
    OUTPUT_NAME "Image"
    PREFIX ""
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../libpng
    ${CMAKE_CURRENT_SOURCE_DIR}/../zlib
    ${CMAKE_CURRENT_SOURCE_DIR}/../BugSlay
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/../sdk/S3TC
)

# S3TC SDK
set(S3TC_LIB "${CMAKE_SOURCE_DIR}/../sdk/S3TC/s3tc.lib")

target_link_libraries(Image
    LibPng
    zlib
    BugSlay
    Misc
    ${S3TC_LIB}
)

target_link_libraries(Image_DLL
    LibPng
    zlib
    BugSlay
    Misc
    ${S3TC_LIB}
)
