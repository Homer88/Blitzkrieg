cmake_minimum_required(VERSION 2.4)

project(PlanePathTest)

# MFC требуется для этого проекта
set(CMAKE_MFC_FLAG 2)  # 1 = static, 2 = shared DLL

# Собираем все необходимые файлы
set(SOURCES
    PlanePathTest.cpp
    PlanePathTestDlg.cpp
    StdAfx.cpp
    PlanePathTest.rc
    src/CManuver.cpp
    src/CManuverBoevoyZahod.cpp
    src/CManuverBuilder.cpp
    src/CManuverContainer.cpp
    src/CManuverGorka.cpp
    src/CManuverHorisontal.cpp
    src/CPlaneRelation.cpp
    src/ComplexPathFraction.cpp
    src/PathFraction.cpp
    src/PlaneFlightExecutor.cpp
    src/PlanePathMath.cpp
)

set(HEADERS
    PlanePathTest.h
    PlanePathTestDlg.h
    Resource.h
    Specific.h
    StdAfx.h
    src/CManuver.h
    src/CManuverBoevoyZahod.h
    src/CManuverBovevoyRazvorot.h
    src/CManuverBuilder.h
    src/CManuverContainer.h
    src/CManuverGorka.h
    src/CManuverHorisontal.h
    src/CManuverPike.h
    src/ComplexPathFraction.h
    src/CPlanePreferences.h
    src/CPlaneRelation.h
    src/IManuver.h
    src/IPlane.h
    src/IPlanePathFraction.h
    src/PathFraction.h
    src/PlaneFlightExecutor.h
    src/PlanePathMath.h
    src/PlanePathTestStubs.h
)

add_executable(PlanePathTest WIN32 ${SOURCES} ${HEADERS})

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../BugSlay
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../AILogic
)

# Определения для MFC и проекта
add_definitions(
    -D_AFXDLL
    -D_MBCS
    -DWIN32
    -D_WINDOWS
)

target_link_libraries(PlanePathTest
    Misc
    BugSlay
)
