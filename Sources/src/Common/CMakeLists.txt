# Common - Общие компоненты
# Без зависимостей

FILE(GLOB COMMON_SOURCES "*.cpp")
FILE(GLOB COMMON_HEADERS "*.h")

add_library(Common STATIC
    ${COMMON_SOURCES}
    ${COMMON_HEADERS}
)

target_compile_definitions(Common PRIVATE
    WIN32
    _WINDOWS
    _LIB
    _CRT_SECURE_NO_WARNINGS
    _WIN32_WINNT=0x0400
)
