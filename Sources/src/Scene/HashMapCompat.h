#ifndef __HASHMAP_COMPAT_H__
#define __HASHMAP_COMPAT_H__

// Файл для совместимости hash_map между MSVC 6.0 и MSVC 2022
// В MSVC 6.0 используется stdext::hash_map с hash_compare
// В MSVC 2022 stdext::hash_map требует std::hash с bucket_size

// Определяем hash функции для основных типов
namespace stdext {
    // hash_compare для DWORD
    struct hash_compare_DWORD {
        size_t operator()(const DWORD& key) const { return (size_t)key; }
        bool operator()(const DWORD& lhs, const DWORD& rhs) const { return lhs < rhs; }
        enum { bucket_size = 4 };
        enum { min_buckets = 8 };
    };

    // hash_compare для std::string
    struct hash_compare_string {
        size_t operator()(const std::string& key) const {
            size_t hash = 0;
            for (size_t i = 0; i < key.length(); ++i)
                hash = hash * 31 + key[i];
            return hash;
        }
        bool operator()(const std::string& lhs, const std::string& rhs) const { return lhs < rhs; }
        enum { bucket_size = 4 };
        enum { min_buckets = 8 };
    };

    // hash_compare для WORD
    struct hash_compare_WORD {
        size_t operator()(const WORD& key) const { return (size_t)key; }
        bool operator()(const WORD& lhs, const WORD& rhs) const { return lhs < rhs; }
        enum { bucket_size = 4 };
        enum { min_buckets = 8 };
    };
}

#endif // __HASHMAP_COMPAT_H__
