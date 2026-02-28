# Минимальная версия CMake
cmake_minimum_required(VERSION 2.4)

# Название проекта
project(LuaLib)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB LUA_LIB_SOURCE "*.cpp")
FILE(GLOB LUA_LIB_HEADERS "*.h")

FILE(GLOB LUA_LIB_SCR_SOURCE "./LuaSrc/*.cpp")
FILE(GLOB LUA_LIB_SCR_HEADERS "./LuaSrc/*.h")


#add_definitions( -D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)
add_definitions( -D_WIN32_WINNT=0x0501)

add_library(LuaLib STATIC ${LUA_LIB_SOURCE} ${LUA_LIB_SCR_SOURCE} ${LUA_LIB_HEADERS} ${LUA_LIB_SCR_HEADERS})

