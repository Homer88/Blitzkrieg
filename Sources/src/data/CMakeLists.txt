# data - Модуль данных
# Зависит от: Misc, StreamIO

cmake_minimum_required(VERSION 2.4)

project(Data)

FILE(GLOB SOURCES "*.cpp")
FILE(GLOB HEADERS "*.h")

add_library(Data STATIC ${SOURCES} ${HEADERS})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/../StreamIO
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)
