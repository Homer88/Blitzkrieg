# Минимальная версия CMake
cmake_minimum_required(VERSION 3.0)

# Название проекта
project( AILogic LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)


#AI логика 
FILE(GLOB AILOGIC_SOURCE "*.cpp")
FILE(GLOB AILOGIC_HEADERS "*.h")

FILE(GLOB SCRIPTS_SOURCE "./Scripts/*.cpp")
FILE(GLOB SCRIPTS_HEADERS "./Scripts/*.h")
add_library (Scripts ${SCRIPTS_SOURCE} ${SCRIPTS_HEADERS})


add_executable(AILogic ${AILOGIC_SOURCE} ${AILOGIC_HEADERS})

target_link_libraries(AILogic   Formats  StreamIOLib zlib BugSlay Image LuaLib RandomMapGen)