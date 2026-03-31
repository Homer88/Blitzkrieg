# Минимальная версия CMake
cmake_minimum_required(VERSION 2.4)

# Название проекта
project(StreamIO)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB STREAMIO_SOURCE "*.cpp")
FILE(GLOB STREAMIO_HEADERS "*.h")

add_definitions( -D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)
add_definitions( -D_WIN32_WINNT=0x0501)

# Функция проверки существования файла
function(check_msxml_installed)
    set(MSXML_PATHS
        "C:/Windows/System32/msxml6.dll"
        "C:/Windows/SysWOW64/msxml6.dll"
        "C:/Windows/System32/msxml3.dll"
        "C:/Windows/SysWOW64/msxml3.dll"
    )
    
    set(MSXML_FOUND FALSE)
    
    foreach(path ${MSXML_PATHS})
        if(EXISTS ${path})
            message(STATUS "Найден MSXML: ${path}")
            set(MSXML_FOUND TRUE PARENT_SCOPE)
            break()
        endif()
    endforeach()
    
    if(NOT MSXML_FOUND)
        message(WARNING "MSXML не найден!")
        set(MSXML_FOUND FALSE PARENT_SCOPE)
    endif()
endfunction()

# Вызываем проверку
check_msxml_installed()

if(NOT MSXML_FOUND)
    message(STATUS "Требуется установка MSXML")
    
    # Вариант 1: просто показать предупреждение
    message(WARNING "Пожалуйста, установите MSXML 6.0 с сайта Microsoft")
    message(FATAL_ERROR "MSXML не установлен. Пожалуйста, установите его вручную.")
endif()


# LIB
#find_library(MSXML_LIB NAMES msxml6 msxml3)
#if(MSXML_LIB)
add_library(StreamIO STATIC ${STREAMIO_HEADERS} ${STREAMIO_SOURCE})

	#target_link_libraries(StreamIO PRIVATE ole32 oleaut32)	
	# DLL
add_library(StreamIO SHARED ${STREAMIO_HEADERS} ${STREAMIO_SOURCE})
	# (Опционально) Устанавливаем версию библиотеки
set_target_properties(StreamIO  PROPERTIES VERSION 1.0.0 SOVERSION 1)
#endif()



