// std_hash_compat.h - Совместимость hash_map/hash_set/hash_multimap
// Включать в файлы ДО первого использования std::hash_map/std::hash_set/std::hash_multimap

#ifndef __STD_HASH_COMPAT_H__
#define __STD_HASH_COMPAT_H__

#if _MSC_VER >= 1900
    // MSVC 2015+: hash_map/hash_set доступны только в stdext
    #include <hash_map>
    #include <hash_set>
    namespace std {
        using stdext::hash_map;
        using stdext::hash_set;
        using stdext::hash_multimap;
    }
#else
    // MSVC 6.0: hash_map/hash_set в stdext или std (зависит от STL)
    #include <hash_map>
    #include <hash_set>
    // Если в MSVC 6 hash_map в stdext, делаем using
    #if defined(_STLP_EXT_DONT_USE_USING)
        // STLPort в пространстве имён std
    #elif !defined(__SGI_STL_INTERNAL_HASH_MAP_H) && defined(stdext)
        namespace std {
            using stdext::hash_map;
            using stdext::hash_set;
            using stdext::hash_multimap;
        }
    #endif
#endif

#endif // __STD_HASH_COMPAT_H__
