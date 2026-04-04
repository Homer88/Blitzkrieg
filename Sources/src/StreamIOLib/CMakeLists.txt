# StreamIOLib - StreamIO библиотека
# Библиотека-заглушка - содержит только заголовочные файлы

cmake_minimum_required(VERSION 2.4)

project(StreamIOLib)

# Создаём пустую библиотеку-заглушку
FILE(GLOB STREAMIOLIB_HEADERS "*.h")

# Создаём минимальный исходник если нет cpp файлов
set(STREAMIOLIB_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/stub.cpp)
file(WRITE ${STREAMIOLIB_SOURCES} "// Auto-generated stub\n")

add_library(StreamIOLib STATIC ${STREAMIOLIB_SOURCES} ${STREAMIOLIB_HEADERS})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)
