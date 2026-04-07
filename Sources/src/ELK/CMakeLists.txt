# ELK - Редактор локализации (MFC Application)
# Зависит от: ELK.lib (из DSP), htmlhelp.lib

cmake_minimum_required(VERSION 2.4)

# Поддержка MFC (динамическая)
set(CMAKE_MFC_FLAG 2)

project(ELK)

# Исходные файлы из DSP
set(SOURCES
    AboutDialog.cpp
    BlitzkriegELKDatabase.cpp
    ChooseFontsDialog.cpp
    CreateFilterDialog.cpp
    CreateFilterNameDialog.cpp
    ELK.cpp
    ELK_Consts.cpp
    ELK_Methods.cpp
    ELK_StaticMethods.cpp
    ELK_TreeWindow.cpp
    ImportFromGameDialog.cpp
    ImportFromPAKDialog.cpp
    ImportFromXLSDialog.cpp
    InputViewDialog.cpp
    InputViewWindow.cpp
    MainFrm.cpp
    ProgressDialog.cpp
    ResizeDialog.cpp
    SpellChecker.cpp
    StatisticDialog.cpp
    TranslateEdit.cpp
    TreeDockWindow.cpp
    GlobalsLoader.cpp
    StdAfx.cpp
)

set(HEADERS
    AboutDialog.h
    BlitzkriegELKDatabase.h
    ChooseFontsDialog.h
    CreateFilterDialog.h
    CreateFilterNameDialog.h
    CSAPI.H
    ELK.h
    ELK_TreeWindow.h
    ELK_Types.h
    htmlhelp.h
    ImportFromGameDialog.h
    ImportFromPAKDialog.h
    ImportFromXLSDialog.h
    InputViewDialog.h
    InputViewWindow.h
    MainFrm.h
    Messages.h
    ProgressDialog.h
    ResizeDialog.h
    SpellChecker.h
    StatisticDialog.h
    TranslateEdit.h
    TreeDockWindow.h
    Resource.h
    Specific.h
    StdAfx.h
)

set(RESOURCE_FILES
    ELK.rc
    res/ELK.rc2
    res/Icons/idr_main.ico
    res/Icons/idr_short_main.ico
    res/Icons/back.ico
    res/Icons/next.ico
    res/Icons/pback.ico
    res/Icons/pnext.ico
    res/Bitmaps/0.bmp
    res/Bitmaps/1.bmp
    res/Bitmaps/2.bmp
    res/Bitmaps/3.bmp
    res/Bitmaps/toolbar.bmp
    res/Bitmaps/toolbarlarge.bmp
    res/Bitmaps/short_toolbar.bmp
    res/Bitmaps/short_toolbarlarge.bmp
    res/Bitmaps/StatisticNormalImageList.bmp
    res/Bitmaps/TreeNormalImageList.bmp
)

# MFC Application (EXE, не DLL/STATIC)
add_executable(ELK WIN32 ${SOURCES} ${HEADERS} ${RESOURCE_FILES})

# Прекомпилированные заголовки
set_target_properties(ELK PROPERTIES
    COMPILE_FLAGS "/YuStdAfx.h"
)
set_source_files_properties(StdAfx.cpp PROPERTIES
    COMPILE_FLAGS "/YcStdAfx.h"
)

# Определения
target_compile_definitions(ELK PRIVATE
    WIN32
    _WINDOWS
    _AFXDLL
    __AFX__
    _MBCS
    _DEBUG
    _WIN32_WINNT=0x0501
    WINVER=0x0501
    _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
)

# Include directories
target_include_directories(ELK PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Link с htmlhelp.lib
target_link_libraries(ELK PRIVATE
    htmlhelp.lib
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
)
