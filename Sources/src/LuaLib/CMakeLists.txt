# LuaLib - Lua библиотека
# Без зависимостей

FILE(GLOB LUALIB_SOURCES "*.cpp")
FILE(GLOB LUALIB_HEADERS "*.h")

add_library(LuaLib STATIC
    ${LUALIB_SOURCES}
    ${LUALIB_HEADERS}
)

target_compile_definitions(LuaLib PRIVATE
    WIN32
    _WINDOWS
    _LIB
    _CRT_SECURE_NO_WARNINGS
    _WIN32_WINNT=0x0400
)

target_include_directories(LuaLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
