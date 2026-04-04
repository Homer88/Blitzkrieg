# RandomMapGen - Генерация случайных карт
# Без зависимостей

FILE(GLOB RANDOMMAPGEN_SOURCES "*.cpp")
FILE(GLOB RANDOMMAPGEN_HEADERS "*.h")

add_library(RandomMapGen STATIC
    ${RANDOMMAPGEN_SOURCES}
    ${RANDOMMAPGEN_HEADERS}
)

target_compile_definitions(RandomMapGen PRIVATE
    WIN32
    _WINDOWS
    _LIB
    _CRT_SECURE_NO_WARNINGS
    _WIN32_WINNT=0x0400
)
