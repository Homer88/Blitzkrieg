#ifndef __HASHFUNCS_H__
#define __HASHFUNCS_H__
//	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma ONCE
//	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct SDefaultPtrHash {
	// fix - C++98 compatible (enum вместо constexpr)
	enum { bucket_size = 16 };

	// Hash function (for stdext::hash_map Traits)
	size_t operator()(const void* pData) const
	{
		return reinterpret_cast<size_t>(pData);
	}

	// Comparison function (for stdext::hash_map Traits)
	// Required by MSVC stdext::hash_map
	bool operator()(const void* a, const void* b) const
	{
		return a < b;
	}
};
//	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct SPtrHash
{
    enum { bucket_size = 16 };

    // Hash function
    template <class T>
    size_t operator()(const CPtr<T>& a) const { return reinterpret_cast<size_t>(a.GetPtr()); }
    
    template <class T>
    size_t operator()(const CObj<T>& a) const { return reinterpret_cast<size_t>(a.GetPtr()); }
    
    // Comparison function
    template <class T>
    bool operator()(const CPtr<T>& a, const CPtr<T>& b) const { return a.GetPtr() < b.GetPtr(); }
    
    template <class T>
    bool operator()(const CObj<T>& a, const CObj<T>& b) const { return a.GetPtr() < b.GetPtr(); }
};
//	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // __HASHFUNCS_H__
