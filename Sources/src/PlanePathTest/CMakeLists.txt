# PlanePathTest - Тест пути самолета
# Зависит от: BugSlay, Misc

cmake_minimum_required(VERSION 2.4)

project(PlanePathTest)

FILE(GLOB SOURCES "*.cpp")
FILE(GLOB HEADERS "*.h")

add_executable(PlanePathTest ${SOURCES} ${HEADERS})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../BugSlay
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(PlanePathTest
    Misc
    BugSlay
)
