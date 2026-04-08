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

# Глобально подключаем пути DirectX SDK (только библиотеки для линковки)
# Заголовки подключаем локально только в модулях, где нужен DirectX (BugSlay, scl, GFX и т.д.)
LINK_DIRECTORIES(${DIRECTX8_LIB_DIR})

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
# BugSlay (исправлен)
add_subdirectory(BugSlay)
add_subdirectory(zlib)
add_subdirectory(libpng)

# === БИБЛИОТЕКИ ПЕРВОГО УРОВНЯ ===
add_subdirectory(Misc)
add_subdirectory(Formats)
add_subdirectory(Common)
add_subdirectory(GameSpy)
add_subdirectory(LuaLib)
add_subdirectory(RandomMapGen)   
add_subdirectory(StreamIOLib)
add_subdirectory(Image)

# === БИБЛИОТЕКИ ВТОРОГО УРОВНЯ ===
add_subdirectory(StreamIO)
add_subdirectory(Input)          # Зависит от BugSlay, Misc, StreamIO
add_subdirectory(Anim)           # Зависит от Image
add_subdirectory(GFX)            # Зависит от BugSlay, Image, Misc, StreamIO, Formats
add_subdirectory(SFX)            # Зависит от BugSlay, Misc, StreamIO
add_subdirectory(Net)
add_subdirectory(scl)
add_subdirectory(UI)             # Зависит от BugSlay, Misc, StreamIO, LuaLib

# === БИБЛИОТЕКИ ТРЕТЬЕГО УРОВНЯ ===
add_subdirectory(Scene)          # Зависит от Image
add_subdirectory(AILogic)        # ИИ логика игры (DLL) - исправляем совместимость с MSVC 2022
add_subdirectory(ELK)            # Редактор локализации (теперь есть Stingray SDK в sdk/Stingray)
add_subdirectory(GameTT)         # Зависит от AILogic

# === БИБЛИОТЕКИ ЧЕТВЁРТОГО УРОВНЯ ===
# add_subdirectory(Main)           # Зависит от AILogic (отключен)
# add_subdirectory(Game)           # Зависит от GameTT (отключен)

# === ПРИЛОЖЕНИЯ ===
add_subdirectory(a7exportmodel)
add_subdirectory(AutoRun)
add_subdirectory(betakeygen)
add_subdirectory(buildversion)
# add_subdirectory(bzmconvertor)   # Зависит от Main
# add_subdirectory(editor)         # Зависит от Main
add_subdirectory(excelexporter)
add_subdirectory(FontGen)
add_subdirectory(imagedefrag)
add_subdirectory(InterfaceSystemTest)
add_subdirectory(MapEditor)      # Зависит от Image
add_subdirectory(PlanePathTest)
add_subdirectory(soundverifycation)  # Утилита проверки звука с FMOD
add_subdirectory(spcomp)

# === УТИЛИТЫ ===
add_subdirectory(Tools)          # Зависит от BugSlay

# ============================================================
message(STATUS "=== Blitzkrieg Build Configuration ===")
message(STATUS "Enabled modules: zlib, Misc, LuaLib, StreamIOLib, StreamIO")
message(STATUS "Disabled: BugSlay + DirectX 8 dependent modules")
message(STATUS "==========================================")
