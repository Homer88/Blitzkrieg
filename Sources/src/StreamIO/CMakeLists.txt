# Минимальная версия CMake
cmake_minimum_required(VERSION 2.4)

# Название проекта
project(StreamIO)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)



# Включить модуль ExternalProject
include(ExternalProject)

# Скачать TinyXML-2
ExternalProject_Add(
    tinyxml2
    GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
    GIT_TAG master
    GIT_SHALLOW TRUE
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
)

#if (NOT TARGET Misc)
 #   add_subdirectory (../Misc misc_build)
  #  endif()

# Получить пути к исходникам
ExternalProject_Get_Property(tinyxml2 source_dir)

# Создать переменную с путем к заголовкам
set(TINYXML2_INCLUDE_DIR ${source_dir})

# Добавить директорию для include (глобально)
include_directories(${TINYXML2_INCLUDE_DIR})

FILE(GLOB STREAMIO_SOURCE "*.cpp")
FILE(GLOB STREAMIO_HEADERS "*.h")

add_definitions( -D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)
add_definitions( -D_WIN32_WINNT=0x0501)
add_definitions( -D_CRT_SECURE_NO_WARNINGS)

# Добавить зависимость для таргета (позже)
add_custom_target(tinyxml2_dep DEPENDS tinyxml2)

# LIB

add_library(StreamIO STATIC ${STREAMIO_HEADERS} ${STREAMIO_SOURCE})

# DLL
#add_library(StreamIO_SHARED SHARED ${STREAMIO_HEADERS} ${STREAMIO_SOURCE})

# Добавить зависимости для библиотек
add_dependencies(StreamIO tinyxml2_dep)
#add_dependencies(StreamIO misc_build)
#add_dependencies(StreamIO_SHARED tinyxml2_dep)
# (Опционально) Устанавливаем версию библиотеки
#set_target_properties(StreamIO  PROPERTIES VERSION 1.0.0 SOVERSION 1)

