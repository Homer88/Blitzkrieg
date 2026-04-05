# Минимальная версия CMake
cmake_minimum_required(VERSION 2.4)

# Название проекта
project(MapEditor)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB MAPEDITOR_SOURCE "*.cpp")
FILE(GLOB MAPEDITOR_HEADERS "*.h")


# для динамической MFC
add_definitions(-D_AFXDLL)
set(CMAKE_MFC_FLAG 2)


add_definitions( -D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)
add_definitions( -D_WIN32_WINNT=0x0501)

add_executable(MapEditor WIN32 ${MAPEDITOR_HEADERS} ${MAPEDITOR_SOURCE})

