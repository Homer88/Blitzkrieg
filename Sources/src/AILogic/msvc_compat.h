// msvc_compat.h - Заголовок для совместимости между MSVC 6.0 и MSVC 2022
// Позволяет собирать проект в обоих компиляторах

#ifndef __MSVC_COMPAT_H__
#define __MSVC_COMPAT_H__

// hash_map/hash_set совместимость — через глобальный std_hash_compat.h
// Включается из StdAfx.h

// Отключение предупреждений о deprecated функциях для MSVC 2022
#if _MSC_VER >= 1900
    #define MODERN_MSVC 1
    #define _CRT_SECURE_NO_WARNINGS
    #define _CRT_NONSTDC_NO_DEPRECATE
    #define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    #define _SILENCE_STDEXT_HASH_MAP_DEPRECATION_WARNINGS
#else
    #define MODERN_MSVC 0
#endif

#endif // __MSVC_COMPAT_H__
