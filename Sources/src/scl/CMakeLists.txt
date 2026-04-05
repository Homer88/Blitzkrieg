# scl - Утилита (Shader Compiler)
# Зависимости: Misc, StreamIOLib, zlib, DirectX 8

cmake_minimum_required(VERSION 2.4)

project(Scl)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB SCL_SOURCE "*.cpp")
FILE(GLOB SCL_HEADERS "*.h")

add_definitions(-D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)

add_library(Scl STATIC ${SCL_SOURCE} ${SCL_HEADERS})

target_include_directories(Scl PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/../StreamIOLib
    ${CMAKE_CURRENT_SOURCE_DIR}/../zlib
)

# ==========================================
# Настройка DirectX 8 SDK (Явная линковка)
# ==========================================

# Вычисляем путь к SDK относительно текущей папки (scl -> ../../sdk/directx)
set(DX_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../sdk/directx")

# Полный путь к заголовкам и библиотеке
set(DX_INC_DIR "${DX_SDK_PATH}/include")
set(D3D_LIB_FILE "${DX_SDK_PATH}/lib/d3d8.lib")

# Выводим путь для отладки
message(STATUS "Scl: Ищем d3d8.lib по пути: ${D3D_LIB_FILE}")

# Подключаем заголовки (include)
if(EXISTS "${DX_INC_DIR}")
    target_include_directories(Scl PRIVATE "${DX_INC_DIR}")
else()
    message(WARNING "Scl: Папка include DirectX не найдена: ${DX_INC_DIR}")
endif()

# Линкуем библиотеку ЯВНО, если она существует
if(EXISTS "${D3D_LIB_FILE}")
    message(STATUS "Scl: Нашел d3d8.lib, линкую...")
    target_link_libraries(Scl 
        "${D3D_LIB_FILE}"
        # Если нужны другие библиотеки DirectX, раскомментируйте:
        # "${DX_SDK_PATH}/lib/d3dx8.lib"
        # "${DX_SDK_PATH}/lib/dxguid.lib"
    )
else()
    message(FATAL_ERROR "Scl: Файл d3d8.lib НЕ НАЙДЕН по пути: ${D3D_LIB_FILE}")
endif()

# Остальные зависимости
target_link_libraries(Scl Misc StreamIOLib zlib)
