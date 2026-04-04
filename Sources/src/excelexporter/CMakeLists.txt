# excelexporter - Экспорт в Excel
# Зависит от: StreamIO, Misc

cmake_minimum_required(VERSION 2.4)

project(ExcelExporter)

FILE(GLOB SOURCES "*.cpp")
FILE(GLOB HEADERS "*.h")

add_executable(ExcelExporter ${SOURCES} ${HEADERS})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../StreamIO
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(ExcelExporter
    StreamIO
    Misc
    BugSlay
    zlib
)
