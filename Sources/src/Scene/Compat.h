#ifndef __COMPAT_H__
#define __COMPAT_H__

// Файл совместимости для компиляции под MSVC 6.0 (Win98/2000) и MSVC 2022 (Win10)
// Определяет макросы для условной компиляции

// Определяем версию компилятора
#if _MSC_VER >= 1400
    // MSVC 2005 и новее (включая MSVC 2022)
    #define _MODERN_MSCPP 1
    #if _MSC_VER >= 1900
        // MSVC 2015+
        #define _HAS_UNORDERED_MAP 1
    #endif
#else
    // MSVC 6.0 и старее
    #define _OLD_MSCPP 1
#endif

// Для совместимости hash_map / unordered_map
#ifdef _MODERN_MSCPP
    // В MSVC 2022 используем unordered_map с hash_map wrapper
    #include <unordered_map>
    #include <unordered_set>
    
    // Эмулируем stdext::hash_map через std::unordered_map
    namespace stdext {
        template<typename K, typename T, typename Hash = std::hash<K>, typename Alloc = std::allocator<std::pair<const K, T>>>
        class hash_map : public std::unordered_map<K, T, Hash, std::equal_to<K>, Alloc> {
        public:
            hash_map() {}
            hash_map(const hash_map& other) : std::unordered_map<K, T, Hash, std::equal_to<K>, Alloc>(other) {}
        };
        
        template<typename K, typename Hash = std::hash<K>>
        struct hash_compare {
            size_t operator()(const K& key) const { return Hash()(key); }
            bool operator()(const K& lhs, const K& rhs) const { return lhs < rhs; }
            enum { bucket_size = 4 };  // Для совместимости со старым API
            enum { min_buckets = 8 };
        };
    }
#else
    // В MSVC 6.0 используем stdext::hash_map
    #include <hash_map>
#endif

// Для push_back() без аргументов - заменяем на emplace_back() в новом MSVC
#ifdef _MODERN_MSCPP
    #define PUSH_BACK_DEFAULT(container) (container).emplace_back()
#else
    #define PUSH_BACK_DEFAULT(container) (container).push_back(typename container::value_type())
#endif

// Макрос for - в C++98 переменные внутри for не видны снаружи
// В старом коде используется хак: #define for if(false); else for
// В новом MSVC это ломает синтаксис, отключаем
#ifdef _MODERN_MSCPP
    #ifdef for
        #undef for
    #endif
    // Используем scope-хак для совместимости
    #define BK_FOR_SCOPE(var) for(var; false; ) {} for(var
#else
    #define BK_FOR_SCOPE(var) for(var
#endif

// PVOID64 конфликт между Bink SDK и Windows SDK
// Решается включением bink.h до windows.h
#ifndef _COMPAT_PVOID64_DEFINED
#define _COMPAT_PVOID64_DEFINED
// Не определяем PVOID64 здесь - это делается в BinkVideoPlayer.h
#endif

#endif // __COMPAT_H__
