//BasicHash.h
#ifdef __BASIC_HASH_H_
#define__BASIC_HASH_H_


// Äëÿ stdext::hash_map è stdext::hash_set
struct SDefaultPtrHashCompare : public stdext::hash_compare<void*>
{
    static const size_t bucket_size = 4;

    size_t operator()(const void* ptr) const
    {
        return reinterpret_cast<size_t>(ptr);
    }

    bool operator()(const void* a, const void* b) const
    {
        return a < b;
    }
};

#endif // 
