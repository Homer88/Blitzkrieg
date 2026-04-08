cmake_minimum_required(VERSION 3.5)

project(MapEditor LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set (SOURCES
    main.cpp
    mainwindow.cpp)
set (HEADERS
    mainwindow.h
    )
set (RESOURCE
    mainwindow.ui)
set (TS_FILES
   Configure_ru_RU.ts )
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCE} ${TS_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
