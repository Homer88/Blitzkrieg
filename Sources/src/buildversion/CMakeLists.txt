# buildversion - Версия сборки
# Зависит от: Misc

cmake_minimum_required(VERSION 2.4)

project(BuildVersion)

FILE(GLOB SOURCES "*.cpp")
FILE(GLOB HEADERS "*.h")

# Статическая библиотека
add_library(BuildVersion STATIC ${SOURCES} ${HEADERS})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)
