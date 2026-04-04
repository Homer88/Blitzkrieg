//BasicHash.h
#ifndef __BASIC_HASH_H_
#define __BASIC_HASH_H_

#include <cstddef>

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

#endif // __BASIC_HASH_H_
