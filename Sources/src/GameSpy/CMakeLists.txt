# GameSpy - GameSpy интеграция
# Без зависимостей

cmake_minimum_required(VERSION 2.4)

project(GameSpy)

FILE(GLOB GAMESPY_SOURCES "*.c")
FILE(GLOB GAMESPY_HEADERS "*.h")

add_library(GameSpy STATIC ${GAMESPY_SOURCES} ${GAMESPY_HEADERS})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)
