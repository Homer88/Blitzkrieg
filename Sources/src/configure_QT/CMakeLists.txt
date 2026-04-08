cmake_minimum_required(VERSION 3.5)

project(Configure LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED Xml)

set(TS_FILES Configure_ru_RU.ts)



set(SOURCES
    main.cpp
    mainwindow.cpp
    cXml.cpp)

set(HEADERS
    mainwindow.h
    cXml.h)

set (RESOURCE
    mainwindow.ui)


add_executable(Configure ${SOURCES} ${HEADERS} ${RESOURCE} ${TS_FILES})
target_link_libraries(Configure PRIVATE Qt5::Widgets Qt5::Xml)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
