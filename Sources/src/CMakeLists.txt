# ============================================================
# Blitzkrieg - Общий проект сборки
# Совместимость: CMake 2.4+ (для сборки под Windows 98/2000)
# Структура основана на A7.dsw
# ============================================================

cmake_minimum_required(VERSION 2.4)

project(Blitzkrieg)

# ============================================================
# Глобальные определения для всех проектов (Windows 98/2000 + новый SDK)
# ============================================================
add_definitions(
    -DWIN32
    -D_WINDOWS
    -D_CRT_SECURE_NO_WARNINGS
    -D_CRT_NONSTDC_NO_DEPRECATE
    -D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    -D_WIN32_WINNT=0x0501
    -DWINVER=0x0501
)

# ============================================================
# DirectX8 SDK из ../sdk/directx
# (Подключается локально только там, где нужен: Scene, GFX, Main и т.д.)
# ============================================================
set(DIRECTX8_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../sdk/directx/include")
set(DIRECTX8_LIB_DIR "${CMAKE_SOURCE_DIR}/../sdk/directx/lib")

if(EXISTS "${DIRECTX8_INCLUDE_DIR}")
    message(STATUS "DirectX8 SDK found: ${DIRECTX8_INCLUDE_DIR}")
endif()

# Глобально НЕ подключаем DirectX, чтобы не ломать утилиты (Misc и др.)
# INCLUDE_DIRECTORIES(${DIRECTX8_INCLUDE_DIR})
# LINK_DIRECTORIES(${DIRECTX8_LIB_DIR})

# Глобальные пути include
INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}
)

# ============================================================
# Загрузка TinyXML-2 для StreamIO
# ============================================================
include(FetchContent)

FetchContent_Declare(
    tinyxml2
    GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)

FetchContent_GetProperties(tinyxml2)
if(NOT tinyxml2_POPULATED)
    FetchContent_Populate(tinyxml2)
    add_subdirectory(${tinyxml2_SOURCE_DIR} ${tinyxml2_BINARY_DIR})
endif()

# ============================================================
# Модули проекта (в порядке зависимостей из A7.dsw)
# ============================================================

# === БАЗОВЫЕ БИБЛИОТЕКИ ===
# BugSlay отключен: конфликт DirectX 8 с Windows SDK 10
# add_subdirectory(BugSlay)
add_subdirectory(zlib)
# add_subdirectory(libpng)  # Требует BugSlay

# === БИБЛИОТЕКИ ПЕРВОГО УРОВНЯ ===
add_subdirectory(Misc)
# add_subdirectory(Formats)        # Требует CPtr исправлений
# add_subdirectory(Common)         # Зависит от BugSlay
# add_subdirectory(GameSpy)        # Зависит от BugSlay
add_subdirectory(LuaLib)
# add_subdirectory(RandomMapGen)   # Зависит от Image -> BugSlay
add_subdirectory(StreamIOLib)
# add_subdirectory(Image)          # Требует BugSlay

# === БИБЛИОТЕКИ ВТОРОГО УРОВНЯ ===
add_subdirectory(StreamIO)
# add_subdirectory(Input)          # Зависит от BugSlay
# add_subdirectory(Anim)           # Зависит от BugSlay
# add_subdirectory(GFX)            # Зависит от BugSlay, DirectX 8
# add_subdirectory(SFX)            # Зависит от BugSlay
# add_subdirectory(UI)             # Зависит от BugSlay
# add_subdirectory(Net)            # Зависит от BugSlay
# add_subdirectory(scl)            # Зависит от BugSlay, DirectX 8

# === БИБЛИОТЕКИ ТРЕТЬЕГО УРОВНЯ ===
# add_subdirectory(Scene)          # Зависит от BugSlay
# add_subdirectory(AILogic)        # Зависит от BugSlay
# add_subdirectory(ELK)            # Зависит от BugSlay
# add_subdirectory(GameTT)         # Зависит от BugSlay

# === БИБЛИОТЕКИ ЧЕТВЁРТОГО УРОВНЯ ===
# add_subdirectory(Main)           # Зависит от BugSlay

# === ПРИЛОЖЕНИЯ ===
# add_subdirectory(A7ExportModel)  # Требует Maya SDK
# add_subdirectory(AutoRun)        # Зависит от BugSlay
# add_subdirectory(betakeygen)     # Зависит от BugSlay
# add_subdirectory(buildversion)   # Зависит от BugSlay
# add_subdirectory(bzmconvertor)   # Зависит от BugSlay
# add_subdirectory(editor)         # Зависит от BugSlay
# add_subdirectory(excelexporter)  # Зависит от BugSlay
# add_subdirectory(FontGen)        # Зависит от BugSlay
# add_subdirectory(Game)           # Зависит от BugSlay
# add_subdirectory(imagedefrag)    # Зависит от BugSlay
# add_subdirectory(InterfaceSystemTest) # Зависит от BugSlay
# add_subdirectory(MapEditor)      # Зависит от BugSlay
# add_subdirectory(PlanePathTest)  # Зависит от BugSlay
# add_subdirectory(soundverifycation) # Требует FMOD
# add_subdirectory(spcomp)         # Зависит от BugSlay

# === УТИЛИТЫ ===
# add_subdirectory(Tools)          # Зависит от BugSlay

# ============================================================
message(STATUS "=== Blitzkrieg Build Configuration ===")
message(STATUS "Enabled modules: zlib, Misc, LuaLib, StreamIOLib, StreamIO")
message(STATUS "Disabled: BugSlay + DirectX 8 dependent modules")
message(STATUS "==========================================")
