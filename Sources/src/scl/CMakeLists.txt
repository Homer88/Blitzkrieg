# scl - Утилита (Shader Compiler?)
# Зависимости: Misc, StreamIOLib, zlib, DirectX 8

cmake_minimum_required(VERSION 2.4)

project(Scl)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB SCL_SOURCE "*.cpp")
FILE(GLOB SCL_HEADERS "*.h")

add_definitions(-D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)

add_executable(Scl ${SCL_SOURCE} ${SCL_HEADERS})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/../StreamIOLib
    ${CMAKE_CURRENT_SOURCE_DIR}/../zlib
)

# DirectX 8 SDK
set(DIRECTX8_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../sdk/directx/include")
set(DIRECTX8_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../sdk/directx/lib")

if(EXISTS "${DIRECTX8_INCLUDE_DIR}")
    INCLUDE_DIRECTORIES(${DIRECTX8_INCLUDE_DIR})
    LINK_DIRECTORIES(${DIRECTX8_LIB_DIR})
    target_link_libraries(Scl d3d8.lib d3dx8.lib dxguid.lib)
endif()

target_link_libraries(Scl Misc StreamIOLib zlib)
