# Минимальная версия CMake
cmake_minimum_required(VERSION 3.5)

# Название проекта
project(LibPng)

# Установка стандарта C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

FILE(GLOB LIBPNG_SOURCE "*.c")
FILE(GLOB LIBPNG_HEADERS "*.h")

# Исключаем файлы с ассемблером, тесты и примеры (не совместимы или не нужны в либе)
list(REMOVE_ITEM LIBPNG_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/pngvcrd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pnggccrd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pngtest.c
    ${CMAKE_CURRENT_SOURCE_DIR}/example.c
)

add_definitions(-D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)

# Общие пути include
set(LIBPNG_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../zlib
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# ==========================================
# Статическая библиотека (.lib)
# ==========================================
add_library(LibPng STATIC ${LIBPNG_SOURCE} ${LIBPNG_HEADERS})
target_include_directories(LibPng PRIVATE ${LIBPNG_INCLUDE_DIRS})

# Явная зависимость от проекта zlib (порядок сборки)
add_dependencies(LibPng zlib)
# Линкуем с библиотекой zlib
target_link_libraries(LibPng zlib)

# ==========================================
# Динамическая библиотека (.dll)
# ==========================================
add_library(LibPng_SHARED SHARED ${LIBPNG_SOURCE} ${LIBPNG_HEADERS})
target_include_directories(LibPng_SHARED PRIVATE ${LIBPNG_INCLUDE_DIRS})

# Явная зависимость от проекта zlib (порядок сборки)
add_dependencies(LibPng_SHARED zlib)
# Линкуем с библиотекой zlib
target_link_libraries(LibPng_SHARED zlib)

# Настройки для DLL:
# 1. Задаем имя выходного файла (чтобы dll называлась LibPng.dll)
set_target_properties(LibPng_SHARED PROPERTIES
    OUTPUT_NAME "LibPng"
    PREFIX ""      # Убираем префикс lib перед именем dll (будет LibPng.dll вместо libLibPng.dll)
    DEFINE_SYMBOL PNG_BUILD_DLL # Определяем макрос для экспорта символов из DLL
)
