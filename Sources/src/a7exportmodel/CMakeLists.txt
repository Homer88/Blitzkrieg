# Минимальная версия CMake
cmake_minimum_required(VERSION 2.4)

# Название проекта
project(A7ExportModel)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB A7ExportModel_SOURCE "*.cpp")
FILE(GLOB A7ExportModel_HEADERS "*.h")

add_definitions( -D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)
add_definitions( -D_WIN32_WINNT=0x0501)

# Maya SDK
set(MAYA_SDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../sdk/Maya4.0")
if(EXISTS "${MAYA_SDK_DIR}")
    message(STATUS "Found Maya SDK at: ${MAYA_SDK_DIR}")
    include_directories(${MAYA_SDK_DIR}/include)
    link_directories(${MAYA_SDK_DIR}/lib)
endif()

add_library(A7ExportModel STATIC ${A7ExportModel_SOURCE} ${A7ExportModel_HEADERS})

target_include_directories(A7ExportModel PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/../StreamIO
    ${CMAKE_CURRENT_SOURCE_DIR}/../Formats
    ${CMAKE_CURRENT_SOURCE_DIR}/../BugSlay
)

target_link_libraries(A7ExportModel BugSlay Misc StreamIO Formats zlib)

