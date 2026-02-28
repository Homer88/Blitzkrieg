# Минимальная версия CMake
cmake_minimum_required(VERSION 3.0)

# Название проекта
project( Blitzkrieg LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#проекты 
add_subdirectory(A7ExportModel)
add_subdirectory(AILogic)

#Анимация 
add_subdirectory(Anim)




add_subdirectory (libpng)
add_subdirectory (LuaLib)
add_subdirectory (net)
add_subdirectory (zlib)
add_subdirectory (AutoRun)

#SDK

# Замена файлов 
FILE (GLOB BINKWINDOWS_SOURCE "./SDK/BINK/*.cpp")
FILE (GLOB BINKWINDOWS_HEADERS "./SDK/BINK/*.h")

#MAYA 4
FILE (GLOB MAYA4_SOURCE "./SDK/MAYA4/*.cpp")
FILE (GLOB MAYA4_HEADERS "./SDK/MAYA4/*.h")

FILE (GLOB S3TC_SOURCE "./SDK/S3TC/*.cpp")
FILE (GLOB S3TC_HEADERS "./SDK/S3TC/*.h")

#STL выбрасываю переписываю на стандартный STL
FILE (GLOB STLPORT_SOURCE "./SDK/stlport/*.cpp")
FILE (GLOB STLPORT_HEADERS "./SDK/stlport/*.h")

#PS2 SDK
FILE (GLOB PS2SDK_HEADERS "./SDK/PS2SDK/include/*.h")
FILE (GLOB PS2SDK_LIBS "./SDK/PS2SDK/lib/*.lib")

#PS3 SDK
FILE (GLOB PS3SDK_HEADERS "./SDK/PS3SDK/include/*.h")
FILE (GLOB PS3SDK_LIBS "./SDK/PS3SDK/lib/*.lib")

#PS4 SDK
FILE (GLOB PS4SDK_HEADERS "./SDK/PS4SDK/include/*.h")
FILE (GLOB PS4SDK_LIBS "./SDK/PS4SDK/lib/*.lib")

#XBOX SDK
FILE (GLOB XBOXSDK_HEADERS "./SDK/XBOXSDK/include/*.h")
FILE (GLOB XBOXSDK_LIBS "./SDK/XBOXSDK/lib/*.lib")
#DirectX 8 SDK
FILE (GLOB DIRECTX8SDK_HEADERS "./SDK/DIRECTX8SDK/include/*.h")
FILE (GLOB DIRECTX8SDK_LIBS "./SDK/DIRECTX8SDK/lib/*.lib")

#DirectX 9 SDK
FILE (GLOB DIRECTX9SDK_HEADERS "./SDK/DIRECTX9SDK/include/*.h")
FILE (GLOB DIRECTX9SDK_LIBS "./SDK/DIRECTX9SDK/lib/*.lib")

#DirectX 10 SDK
FILE (GLOB DIRECTX10SDK_HEADERS "./SDK/DIRECTX10SDK/include/*.h")
FILE (GLOB DIRECTX10SDK_LIBS "./SDK/DIRECTX10SDK/lib/*.lib")

#DirectX 11 SDK
FILE (GLOB DIRECTX11SDK_HEADERS "./SDK/DIRECTX11SDK/include/*.h")
FILE (GLOB DIRECTX11SDK_LIBS "./SDK/DIRECTX11SDK/lib/*.lib")

#DirectX 12 SDK
FILE (GLOB DIRECTX12SDK_HEADERS "./SDK/DIRECTX12SDK/include/*.h")
FILE (GLOB DIRECTX12SDK_LIBS "./SDK/DIRECTX12SDK/lib/*.lib")

#SDL SDK
FILE (GLOB SDLSDK_HEADERS "./SDK/SDLSDK/include/*.h")
FILE (GLOB SDLSDK_LIBS "./SDK/SDLSDK/lib/*.lib")

#SMFL SDK
FILE (GLOB SMFLSDK_HEADERS "./SDK/SMFLSDK/include/*.h")
FILE (GLOB SMFLSDK_LIBS "./SDK/SMFLSDK/lib/*.lib")

#ZIP библиотеки 

#Windows9X
if(WIN9X)
    message(STATUS "Настройка для Windows 9X")
    # Можно подключить специфичные для Windows библиотеки или опции
	add_subdirectory (zlib)
    add_definitions(-DWINDOWS_SPECIFIC)
    list(APPEND SOURCES windows_specific.cpp)
endif()

#DirectX9
if(WINX86D9)
    message(STATUS "Настройка для Windows X86 DirectX 9")
    # Можно подключить специфичные для Windows библиотеки или опции
    add_definitions(-DWINDOWS_SPECIFIC)
    list(APPEND SOURCES windows_specific.cpp)
endif()

#DirectX10
if(WINX86D10)
    message(STATUS "Настройка для Windows X86 DirectX 10")
    # Можно подключить специфичные для Windows библиотеки или опции
    add_definitions(-DWINDOWS_SPECIFIC)
    list(APPEND SOURCES windows_specific.cpp)
endif()
#DirectX11
if(WINX86D11)
    message(STATUS "Настройка для Windows X86 DirectX 11")
    # Можно подключить специфичные для Windows библиотеки или опции
    add_definitions(-DWINDOWS_SPECIFIC)
    list(APPEND SOURCES windows_specific.cpp)
endif()

#DirectX12
if(WINX86D12)
    message(STATUS "Настройка для Windows X86 DirectX 12")
    # Можно подключить специфичные для Windows библиотеки или опции
    add_definitions(-DWINDOWS_SPECIFIC)
    list(APPEND SOURCES windows_specific.cpp)
endif()
#OpenGL
if(WINX86GL)
    message(STATUS "Настройка для Windows X86 OpenGL")
    # Можно подключить специфичные для Windows библиотеки или опции
    add_definitions(-DWINDOWS_SPECIFIC)
    list(APPEND SOURCES windows_specific.cpp)
endif()
#Windows X64
#DirectX9
if(WINX64D9)
    message(STATUS "Настройка для Windows X64 DirectX 9")
    # Можно подключить специфичные для Windows библиотеки или опции
    add_definitions(-DWINDOWS_SPECIFIC)
    list(APPEND SOURCES windows_specific.cpp)
endif()
#DirectX10
if(WINX64D10)
    message(STATUS "Настройка для Windows X64 DirectX 10")
    # Можно подключить специфичные для Windows библиотеки или опции
    add_definitions(-DWINDOWS_SPECIFIC)
    list(APPEND SOURCES windows_specific.cpp)
endif()
#DirectX11
if(WINX64D11)
    message(STATUS "Настройка для Windows X64 DirectX 11")
    # Можно подключить специфичные для Windows библиотеки или опции
    add_definitions(-DWINDOWS_SPECIFIC)
    list(APPEND SOURCES windows_specific.cpp)
endif()

#DirectX12
if(WINX64D12)
    message(STATUS "Настройка для Windows X64 DirectX 12")
    # Можно подключить специфичные для Windows библиотеки или опции
    add_definitions(-DWINDOWS_SPECIFIC)
    list(APPEND SOURCES windows_specific.cpp)
endif()
#OpenGL
if(WINX64GL)
    message(STATUS "Настройка для Windows X64 OpenGL")
    # Можно подключить специфичные для Windows библиотеки или опции
    add_definitions(-DWINDOWS_SPECIFIC)
    list(APPEND SOURCES windows_specific.cpp)
endif()


#PS2

if(PS2)
    message(STATUS "Настройка для PS2")
    # Можно подключить специфичные для PS3 библиотеки или опции
    add_definitions(-DWINDOWS_SPECIFIC)
    list(APPEND SOURCES windows_specific.cpp)
endif()

#PS3
if(PS3)
    message(STATUS "Настройка для PS3")
    # Можно подключить специфичные для PS3 библиотеки или опции
    add_definitions(-DWINDOWS_SPECIFIC)
    list(APPEND SOURCES windows_specific.cpp)
endif()

#PS4
if(PS4)
    message(STATUS "Настройка для PS4")
    # Можно подключить специфичные для PS4 библиотеки или опции
    add_definitions(-DWINDOWS_SPECIFIC)
    list(APPEND SOURCES windows_specific.cpp)
endif()

#XBOX
if(XBOX)
    message(STATUS "Настройка для XBOX")
    # Можно подключить специфичные для XBOX библиотеки или опции
    add_definitions(-DWINDOWS_SPECIFIC)
    list(APPEND SOURCES windows_specific.cpp)
endif()

#Linux X86
if(LINX86)
    message(STATUS "Настройка для LINUX X86")
    # Можно подключить специфичные для Linux библиотеки или опции
    add_definitions(-DWINDOWS_SPECIFIC)
    list(APPEND SOURCES windows_specific.cpp)
endif()
if(LINX86SDL)
    message(STATUS "Настройка для LINUX X86")
    # Можно подключить специфичные для Linux библиотеки или опции
    add_definitions(-DWINDOWS_SPECIFIC)
    list(APPEND SOURCES windows_specific.cpp)
endif()

if(LINX86SMFL)
    message(STATUS "Настройка для LINUX X86")
    # Можно подключить специфичные для Linux библиотеки или опции
    add_definitions(-DWINDOWS_SPECIFIC)
    list(APPEND SOURCES windows_specific.cpp)
endif()

#Linux X64
if(LINX64)
    message(STATUS "Настройка для LINUX X64")
    # Можно подключить специфичные для Linux библиотеки или опции
    add_definitions(-DWINDOWS_SPECIFIC)
    list(APPEND SOURCES windows_specific.cpp)
endif()

if(LINX64SDL)
    message(STATUS "Настройка для LINUX X64 SDL")
    # Можно подключить специфичные для Linux библиотеки или опции
    add_definitions(-DWINDOWS_SPECIFIC)
    list(APPEND SOURCES windows_specific.cpp)
endif()

if(LINX64SMLF)
    message(STATUS "Настройка для LINUX X64 SMFL")
    # Можно подключить специфичные для Linux библиотеки или опции
    add_definitions(-DWINDOWS_SPECIFIC)
    list(APPEND SOURCES windows_specific.cpp)
endif()
