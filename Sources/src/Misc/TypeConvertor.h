#ifndef __TYPECONVERTOR_H__
#define __TYPECONVERTOR_H__
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma ONCE
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// for adding to type converter enums in way STRING_ENUM_ADDER( converter, ENUM )
#define STRING_ENUM_ADD_PTR(TypeConverter,eEnum) (*TypeConverter)[#eEnum] = eEnum;
#define STRING_ENUM_ADD(TypeConverter,eEnum) TypeConverter.Add( #eEnum, eEnum );
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Для совместимости с MSVC 2022 - stdext::hash_map требует hash_compare с bucket_size
// Определяем hash_compare для основных типов
#if _MSC_VER >= 1400
namespace NCompat {
    template<typename T>
    struct THash {
        size_t operator()(const T& v) const { return (size_t)v; }
        bool operator()(const T& a, const T& b) const { return a < b; }
        enum { bucket_size = 4 };
        enum { min_buckets = 8 };
    };
    template<>
    struct THash<std::string> {
        size_t operator()(const std::string& s) const {
            size_t h = 0;
            for (size_t i = 0; i < s.length(); ++i)
                h = h * 31 + s[i];
            return h;
        }
        bool operator()(const std::string& a, const std::string& b) const { return a < b; }
        enum { bucket_size = 4 };
        enum { min_buckets = 8 };
    };
    template<>
    struct THash<WORD> {
        size_t operator()(const WORD& v) const { return (size_t)v; }
        bool operator()(const WORD& a, const WORD& b) const { return a < b; }
        enum { bucket_size = 4 };
        enum { min_buckets = 8 };
    };
    template<>
    struct THash<DWORD> {
        size_t operator()(const DWORD& v) const { return (size_t)v; }
        bool operator()(const DWORD& a, const DWORD& b) const { return a < b; }
        enum { bucket_size = 4 };
        enum { min_buckets = 8 };
    };
}
#define DEFAULT_HASH1 NCompat::THash<T1>
#define DEFAULT_HASH2 NCompat::THash<T2>
#else
#define DEFAULT_HASH1 std::hash<T1>
#define DEFAULT_HASH2 std::hash<T2>
#endif

template < class T1, class T2, class TH1 = DEFAULT_HASH1, class TH2 = DEFAULT_HASH2 >
class CTypeConvertor
{
protected: // ������������� �������.
	std::hash_map<T1, T2, TH1> t1_t2;
	std::hash_map<T2, T1, TH2> t2_t1;
	//
	const T1& GetType1( const T2 &t2 ) const
	{
		std::hash_map<T2, T1, TH2>::const_iterator pos = t2_t1.find( t2 );
		NI_ASSERT_T( pos != t2_t1.end(), "Can't find type1 from type2" );
		return pos->second;
	}
	const T2& GetType2( const T1 &t1 ) const
	{
		std::hash_map<T1, T2, TH1>::const_iterator pos = t1_t2.find( t1 );
		NI_ASSERT_T( pos != t1_t2.end(), "Can't find type2 from type1" );
		return pos->second;
	}
public:
	//
	void Add( const T1 &t1, const T2 &t2 ) { t1_t2[t1] = t2; t2_t1[t2] = t1; }
	void Remove( const T1 &t1 ) { const T2 &t2 = GetType2( t1 ); t2_t1.erase( t2 ); t1_t2.erase( t1 ); }
	void Remove( const T2 &t2 ) { const T1 &t1 = GetType1( t2 ); t1_t2.erase( t1 ); t2_t1.erase( t2 ); }
	//
	const T2& ToT2( const T1 &t1 ) const { return GetType2( t1 ); }
	const T1& ToT1( const T2 &t2 ) const { return GetType1( t2 ); }
	bool IsPresent( const T1 &t1 ) const { return t1_t2.find( t1 ) != t1_t2.end(); }
	bool IsPresent( const T2 &t2 ) const { return t2_t1.find( t2 ) != t2_t1.end(); }

	bool IsEmpty() const { return t1_t2.empty() || t2_t1.empty(); }
	void Clear() { t1_t2.clear(); t2_t1.clear(); }
	int operator&( IStructureSaver &ss )
	{
		CSaverAccessor saver = &ss;
		saver.Add( 1, &t1_t2 );
		saver.Add( 2, &t2_t1 );
		return 0;
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // __TYPECONVERTOR_H__
