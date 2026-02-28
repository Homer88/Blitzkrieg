# Минимальная версия CMake
cmake_minimum_required(VERSION 2.4)

# Название проекта
project(AutoRun)

if (NOT TARGET zlib)
    add_subdirectory (../zlib zlib_build)
    endif()

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# для динамической MFC
add_definitions(-D_AFXDLL)
set(CMAKE_MFC_FLAG 2)






#Autorun логика 
FILE(GLOB AUTORUN_SOURCE "*.cpp")
FILE(GLOB AUTORUN_HEADERS "*.h")
 
add_definitions( -D_CRT_SECURE_NO_WARNINGS)
add_definitions( -D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)
add_definitions( -D_WIN32_WINNT=0x0501)

add_executable( AutoRun WIN32 ${AUTORUN_SOURCE} ${AUTORUN_HEADERS} AutoRun.rc )
target_link_libraries(AutoRun PRIVATE zlib)
target_link_libraries(AutoRun PRIVATE  winmm.lib)

