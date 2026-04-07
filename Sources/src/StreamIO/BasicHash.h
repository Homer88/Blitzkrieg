//BasicHash.h
#ifndef __BASIC_HASH_H_
#define __BASIC_HASH_H_

#include <cstddef>

#if _MSC_VER >= 1600
// MSVC 2010 и выше - используем std::unordered_map
#include <unordered_map>
#define BLITZ_HASH_MAP std::unordered_map
#define BLITZ_HASH_SET std::unordered_set
#else
// MSVC 2002-2008 - используем stdext::hash_map
#include <hash_map>
#define BLITZ_HASH_MAP stdext::hash_map
#define BLITZ_HASH_SET stdext::hash_set
#endif

// Для stdext::hash_map и stdext::hash_set
// Хеш-функтор для указателей (совместим с C++98)
struct SDefaultPtrHashCompare
{
    // C++98 compatible - enum вместо static const
    enum { bucket_size = 4 };

    // Оператор хеширования (унарный)
    size_t operator()(const void* ptr) const
    {
        // C++98 совместимое хеширование указателя
        return reinterpret_cast<size_t>(ptr);
    }

    // Оператор сравнения (бинарный) для упорядочивания
    bool operator()(const void* a, const void* b) const
    {
        return a < b;
    }
};

// Шаблон для создания хеш-функтора с bucket_size
// Использование: DECLARE_HASH_FUNC(Имя, Тип, хеш_выражение)
#define DECLARE_HASH_FUNC(Name, KeyType, HashExpr) \
struct Name \
{ \
    enum { bucket_size = 4 }; \
    size_t operator()(const KeyType& key) const { return HashExpr; } \
    bool operator()(const KeyType& a, const KeyType& b) const { return a < b; } \
}

#endif // __BASIC_HASH_H_
