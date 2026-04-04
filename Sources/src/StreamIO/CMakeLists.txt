# =============================================================================
# CMakeLists.txt для проекта StreamIO
# Совместимость: CMake 2.4+ (для сборки под Windows 98/2000)
# =============================================================================
cmake_minimum_required(VERSION 2.4)

project(StreamIO)

# =============================================================================
# Настройки компилятора
# =============================================================================
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS -D_CRT_SECURE_NO_WARNINGS")

# Windows-specific определения
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINNT=0x0501")
endif()

# =============================================================================
# Сбор исходных файлов
# =============================================================================
FILE(GLOB STREAMIO_SOURCES "*.cpp")
FILE(GLOB STREAMIO_HEADERS "*.h")

# Исключаем конфликтующие файлы из сборки
list(REMOVE_ITEM STREAMIO_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Singleton.cpp
)

# =============================================================================
# Include директории
# =============================================================================
INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/../Main
)

# =============================================================================
# Зависимости: TinyXML-2 и zlib (должны быть установлены пользователем)
# =============================================================================
set(TINYXML2_INCLUDE_DIR "" CACHE PATH "Path to TinyXML-2 include directory")
set(ZLIB_INCLUDE_DIR "" CACHE PATH "Path to zlib include directory")

if(TINYXML2_INCLUDE_DIR)
    INCLUDE_DIRECTORIES(${TINYXML2_INCLUDE_DIR})
endif()

if(ZLIB_INCLUDE_DIR)
    INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})
endif()

# =============================================================================
# Статическая библиотека
# =============================================================================
add_library(StreamIO_STATIC STATIC ${STREAMIO_HEADERS} ${STREAMIO_SOURCES})
set_target_properties(StreamIO_STATIC PROPERTIES OUTPUT_NAME "StreamIO_static")

# =============================================================================
# DLL (основная библиотека)
# =============================================================================
add_library(StreamIO SHARED ${STREAMIO_HEADERS} ${STREAMIO_SOURCES})

# Используем .def файл для экспорта функций (Windows)
if(WIN32)
    set_target_properties(StreamIO PROPERTIES
        DEFINE_FILE "StreamIO.def"
    )
endif()

set_target_properties(StreamIO PROPERTIES
    OUTPUT_NAME "StreamIO"
)

# =============================================================================
# Зависимость от Misc (собирается в корневом проекте)
# =============================================================================
# Misc уже добавлен в корневом CMakeLists.txt, просто линкуемся.
target_link_libraries(StreamIO Misc)
target_link_libraries(StreamIO_STATIC Misc)

# =============================================================================
# Линкуем с TinyXML-2 (из FetchContent)
# =============================================================================
if(TARGET tinyxml2)
    target_link_libraries(StreamIO tinyxml2)
    target_link_libraries(StreamIO_STATIC tinyxml2)
    target_include_directories(StreamIO PRIVATE ${tinyxml2_SOURCE_DIR})
    target_include_directories(StreamIO_STATIC PRIVATE ${tinyxml2_SOURCE_DIR})
endif()

# =============================================================================
# Линкуем с zlib (из корневого проекта)
# =============================================================================
if(TARGET zlib)
    target_link_libraries(StreamIO zlib)
    target_link_libraries(StreamIO_STATIC zlib)
endif()

# =============================================================================
# Платформо-зависимые библиотеки
# =============================================================================
if(WIN32)
    target_link_libraries(StreamIO comsuppw.lib winmm.lib)
elseif(UNIX)
    target_link_libraries(StreamIO dl pthread)
endif()

# =============================================================================
# Установка выходных директорий
# =============================================================================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# =============================================================================
# Инструкция для пользователя
# =============================================================================
message("")
message("=== StreamIO Build Instructions ===")
message("Для сборки с внешними зависимостями:")
message("  cmake -DTINYXML2_INCLUDE_DIR=<path> -DTINYXML2_LIB=<lib> -DZLIB_INCLUDE_DIR=<path> -DZLIB_LIB=<lib> .")
message("")
