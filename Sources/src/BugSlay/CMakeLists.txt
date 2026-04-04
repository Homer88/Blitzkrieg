# BugSlay - Базовая библиотека утилит (с DirectX 8)
# Зависимости: Misc

cmake_minimum_required(VERSION 2.4)

project(BugSlay)

FILE(GLOB BUGSLAY_SOURCES "*.cpp")
FILE(GLOB BUGSLAY_HEADERS "*.h")

# Исключаем файлы с ассемблером (не совместимы с x64)
list(REMOVE_ITEM BUGSLAY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/DumbPow2Alloc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MemorySystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/STLDebugTerminate.cpp
)

add_library(BugSlay STATIC ${BUGSLAY_SOURCES} ${BUGSLAY_HEADERS})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
)

# DirectX 8 SDK (локально только для BugSlay)
set(DIRECTX8_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../sdk/directx/include")
set(DIRECTX8_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../sdk/directx/lib")

if(EXISTS "${DIRECTX8_INCLUDE_DIR}")
    INCLUDE_DIRECTORIES(${DIRECTX8_INCLUDE_DIR})
    LINK_DIRECTORIES(${DIRECTX8_LIB_DIR})
    target_link_libraries(BugSlay d3d8.lib d3dx8.lib dxguid.lib)
endif()
