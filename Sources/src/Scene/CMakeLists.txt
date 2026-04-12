# Scene - Сцена, рендеринг, камера, курсор, частицы, terrain, видео (DLL + STATIC)
# Зависит от: Image, BugSlay, Misc, StreamIO, Common, Formats, Anim, Input, SFX, GFX

cmake_minimum_required(VERSION 2.4)

project(Scene)

# Установка стандарта C++98 для совместимости с Windows 98/2000
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Определение исходных файлов и заголовков
FILE(GLOB SCENE_SOURCES "*.cpp")
FILE(GLOB SCENE_HEADERS "*.h")

# Исключаем BinkVideoPlayer.cpp - не совместим с Windows SDK 10 / MSVC 2022
# Bink SDK 2002 года требует старые заголовки Windows
list(REMOVE_ITEM SCENE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/BinkVideoPlayer.cpp")

# Ресурсы (курсоры и rc файл)
set(SCENE_RESOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Scene.rc
    ${CMAKE_CURRENT_SOURCE_DIR}/Scene.def
)

# Создаем STATIC библиотеку
add_library(Scene_STATIC STATIC ${SCENE_SOURCES} ${SCENE_HEADERS} ${SCENE_RESOURCES})

# Создаем DLL
add_library(Scene SHARED ${SCENE_SOURCES} ${SCENE_HEADERS} ${SCENE_RESOURCES})

# Определение файла .def для экспорта
set_target_properties(Scene PROPERTIES
    LINK_FLAGS "/DEF:\"${CMAKE_CURRENT_SOURCE_DIR}/Scene.def\""
)

# Установка имени выходного файла
set_target_properties(Scene_STATIC PROPERTIES
    OUTPUT_NAME "Scene_static"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)

set_target_properties(Scene PROPERTIES
    OUTPUT_NAME "Scene"
    PREFIX ""
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

# SDK пути
set(BINK_SDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../sdk/bink")
set(DIRECTX8_SDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../sdk/directx")

# Пути include
INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../BugSlay
    ${CMAKE_CURRENT_SOURCE_DIR}/../Misc
    ${CMAKE_CURRENT_SOURCE_DIR}/../StreamIO
    ${CMAKE_CURRENT_SOURCE_DIR}/../Common
    ${CMAKE_CURRENT_SOURCE_DIR}/../Formats
    ${CMAKE_CURRENT_SOURCE_DIR}/../Anim
    ${CMAKE_CURRENT_SOURCE_DIR}/../Image
    ${CMAKE_CURRENT_SOURCE_DIR}/../Input
    ${CMAKE_CURRENT_SOURCE_DIR}/../SFX
    ${CMAKE_CURRENT_SOURCE_DIR}/../GFX
    ${CMAKE_CURRENT_SOURCE_DIR}/../Main
    ${BINK_SDK_DIR}
    ${DIRECTX8_SDK_DIR}/include
)

# Отключаем макрос for из Misc/Basic.h который ломает синтаксис C++11+
add_compile_options(/Dfor=for)

# Пути для библиотек
LINK_DIRECTORIES(
    ${BINK_SDK_DIR}
    ${DIRECTX8_SDK_DIR}/lib
)

# Компиляторные определения для Windows 98/2000
target_compile_definitions(Scene_STATIC PRIVATE
    WIN32
    _WINDOWS
    _LIB
    _CRT_SECURE_NO_WARNINGS
    _WIN32_WINNT=0x0501
    WINVER=0x0501
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    # Отключаем Bink Video - требует проприетарный SDK который не совместим с MSVC 2022
    _NO_BINK_VIDEO
)

target_compile_definitions(Scene PRIVATE
    WIN32
    _WINDOWS
    _USRDLL
    _CRT_SECURE_NO_WARNINGS
    _WIN32_WINNT=0x0501
    WINVER=0x0501
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    # Отключаем Bink Video - требует проприетарный SDK который не совместим с MSVC 2022
    _NO_BINK_VIDEO
)

# Определения для разных конфигуций сборки
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(Scene_STATIC PRIVATE _FINALRELEASE NDEBUG)
    target_compile_definitions(Scene PRIVATE _FINALRELEASE NDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Scene_STATIC PRIVATE _DEBUG _DO_ASSERT_SLOW _DO_CHECKED_CAST _STL_RANGE_CHECK)
    target_compile_definitions(Scene PRIVATE _DEBUG _DO_ASSERT_SLOW _DO_CHECKED_CAST _STL_RANGE_CHECK)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    target_compile_definitions(Scene_STATIC PRIVATE _BETARELEASE NDEBUG _DO_ASSERT_SLOW)
    target_compile_definitions(Scene PRIVATE _BETARELEASE NDEBUG _DO_ASSERT_SLOW)
endif()

# Связывание библиотек
# Используем абсолютный путь к binkw32.lib напрямую
set(BINKW32_LIB "${BINK_SDK_DIR}/binkw32.lib")

target_link_libraries(Scene_STATIC
    # Мультимедиа и видео
    winmm
    ${BINKW32_LIB}

    # Зависимости от других модулей проекта
    BugSlay
    Misc
    StreamIO_STATIC
    Common
    Formats
    Image
    Input
    # Anim - not linked, loaded dynamically at runtime
    SFX_STATIC
    GFX_STATIC
    UI_STATIC
    zlib
    libpng

    # Системные библиотеки Windows
    kernel32
    user32
    gdi32
    winspool
    comdlg32
    advapi32
    shell32
    ole32
    oleaut32
    uuid
    odbc32
    odbccp32

    # COM Support
    comsupp
)

target_link_libraries(Scene
    # Мультимедиа и видео
    winmm
    ${BINKW32_LIB}

    # Зависимости от других модулей проекта
    BugSlay
    Misc
    StreamIO
    Common
    Formats
    Image_DLL
    Input_DLL
    # Anim - not linked, loaded dynamically at runtime
    SFX
    GFX
    UI
    zlib
    libpng

    # Системные библиотеки Windows
    kernel32
    user32
    gdi32
    winspool
    comdlg32
    advapi32
    shell32
    ole32
    oleaut32
    uuid
    odbc32
    odbccp32

    # COM Support
    comsupp
)
