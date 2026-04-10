# GFX - Графический движок (DLL)
# Зависимости: BugSlay, Image, Misc, StreamIO, Formats

cmake_minimum_required(VERSION 2.4)
project(GFX)

# Опредделения компиляции из DSP
add_definitions(
    -DWIN32
    -D_WINDOWS
    -D_DEBUG
    -D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    -D_WIN32_WINNT=0x0501
    -D_CRT_SECURE_NO_WARNINGS
)

# Исходные файлы из GFX.dsp (только те, что указаны в DSP)
set(GFX_SOURCES
    # Common
    StdAfx.cpp
    GlobalsLoader.cpp

    # GFX
    GFXObjectFactory.cpp
    GraphicsEngine.cpp
    VideoCheck.cpp

    # Texture
    Texture.cpp
    TextureManager.cpp

    # Geometry
    GeometryBuffer.cpp
    GeometryManager.cpp
    GeometryMesh.cpp
    RangeAllocs.cpp

    # Clipping
    Clipping.cpp

    # Font
    Font.cpp
    FontManager.cpp
    GFXTextVisitors.cpp
    Text.cpp
)

# Заголовки
FILE(GLOB GFX_HEADERS "*.h")

# Создаём DLL
add_library(GFX SHARED ${GFX_SOURCES} ${GFX_HEADERS} GFX.rc)

# Используем .def файл для экспорта
if(WIN32)
    target_link_options(GFX PRIVATE "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/GFX.def")
endif()

# Прекомпилированные заголовки
set_target_properties(GFX PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
)

# wchar_t как unsigned short (совместимость с остальным проектом)
target_compile_options(GFX PRIVATE /Zc:wchar_t-)

# Отключаем SAFESEH для совместимости со старой d3d8.lib
target_link_options(GFX PRIVATE /SAFESEH:NO)

set_source_files_properties(StdAfx.cpp PROPERTIES
    COMPILE_FLAGS "/YcStdAfx.h"
)

# Include директории
target_include_directories(GFX PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/BugSlay
    ${CMAKE_SOURCE_DIR}/Image
    ${CMAKE_SOURCE_DIR}/Misc
    ${CMAKE_SOURCE_DIR}/StreamIO
    ${CMAKE_SOURCE_DIR}/Formats
    # DirectX8 SDK
    ${CMAKE_SOURCE_DIR}/../sdk/directx/include
)

# Зависимости
target_link_libraries(GFX
    Image
    Misc
    BugSlay
    StreamIO
    Formats
    # DirectX8
    d3d8.lib
    dxguid.lib
    # Системные
    kernel32.lib
    user32.lib
    gdi32.lib
    winspool.lib
    comdlg32.lib
    advapi32.lib
    shell32.lib
    ole32.lib
    oleaut32.lib
    uuid.lib
    odbc32.lib
    odbccp32.lib
    comsupp.lib
)
