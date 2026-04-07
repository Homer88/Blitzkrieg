// msvc_compat.h - Заголовок для совместимости между MSVC 6.0 и MSVC 2022
// Позволяет собирать проект в обоих компиляторах

#ifndef __MSVC_COMPAT_H__
#define __MSVC_COMPAT_H__

// Определение версии компилятора
// MSVC 6.0   = _MSC_VER 1200
// MSVC 2022  = _MSC_VER >= 1930

#if _MSC_VER >= 1900
    // MSVC 2015+ (C++11 поддержка, <hash_map> удалён)
    #define MODERN_MSVC 1
    
    // unordered_map/unordered_set вместо hash_map/hash_set
    #include <unordered_map>
    #include <unordered_set>
    namespace stdext {
        template<typename K, typename V, typename H = std::hash<K>, typename P = std::equal_to<K>, 
                 typename A = std::allocator<std::pair<const K, V>>>
        using hash_map = std::unordered_map<K, V, H, P, A>;
        
        template<typename K, typename H = std::hash<K>, typename P = std::equal_to<K>,
                 typename A = std::allocator<K>>
        using hash_set = std::unordered_set<K, H, P, A>;
    }
    
    // bind2nd удалён в C++17 - используем lambdas
    // Для кода, который использует bind2nd, нужны #ifdef в самих .cpp файлах
    
    // push_back() без аргументов больше не работает - нужен явный аргумент
    // Это требует исправления в исходном коде

#else
    // MSVC 6.0
    #define MODERN_MSVC 0
    
    // hash_map/hash_set в stdext
    #include <hash_map>
    #include <hash_set>
    
#endif

// Отключение предупреждений о deprecated функциях для старого кода
#if MODERN_MSVC
    #define _CRT_SECURE_NO_WARNINGS
    #define _CRT_NONSTDC_NO_DEPRECATE
    #define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#endif

#endif // __MSVC_COMPAT_H__
