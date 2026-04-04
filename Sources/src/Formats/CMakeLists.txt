# Formats - Форматы файлов
# Без зависимостей

FILE(GLOB FORMATS_SOURCES "*.cpp")
FILE(GLOB FORMATS_HEADERS "*.h")

add_library(Formats STATIC
    ${FORMATS_SOURCES}
    ${FORMATS_HEADERS}
)

target_compile_definitions(Formats PRIVATE
    WIN32
    _WINDOWS
    _LIB
    _CRT_SECURE_NO_WARNINGS
    _WIN32_WINNT=0x0400
)
