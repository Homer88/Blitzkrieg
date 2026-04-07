#ifndef __AI_HASH_FUNCS__
#define __AI_HASH_FUNCS__
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma ONCE
#include "StreamIO/BasicHash.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
interface IUpdatableObj;			// определён в UpdatableObject.h
class CAIUnit;						// определён в AIUnit.h
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Хеш-функторы для hash_map/hash_set
// Содержат и operator() для хеширования, и operator() для сравнения
struct SUpdatableObjectObjHash
{
	enum { bucket_size = 4 };
	size_t operator()( const CObj<IUpdatableObj> &a ) const { return (size_t)a.GetPtr()->GetUniqueId(); }
	bool operator()( const CObj<IUpdatableObj> &a, const CObj<IUpdatableObj> &b ) const { return a.GetPtr() < b.GetPtr(); }
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Хеш-функтор для CAIUnit — определён в AIUnit.h (после полного определения CAIUnit)
// struct SUnitObjHash перенесён в AIUnit.h чтобы избежать circular include
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct SUniqueIdHash
{
	enum { bucket_size = 4 };
	template <class T>
		size_t operator()( const CObj<T> &a ) const { return (size_t)a.GetPtr()->GetUniqueId(); }
	template <class T>
		size_t operator()( const T *a ) const { return (size_t)a->GetUniqueId(); }
	template <class T>
		bool operator()( const CObj<T> &a, const CObj<T> &b ) const { return a.GetPtr() < b.GetPtr(); }
	template <class T>
		bool operator()( const T *a, const T *b ) const { return a < b; }
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct STilesHash
{
	enum { bucket_size = 4 };
	size_t operator()( const SVector &tile ) const
	{
		return (size_t)( ( tile.x << 12 ) | tile.y );
	}
	bool operator()( const SVector &a, const SVector &b ) const { return a < b; }
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct SVec2Hash
{
	enum { bucket_size = 4 };
	size_t operator()( const CVec2 &pos) const
	{
		return (size_t)( (int(pos.x)<<16) | int(pos.y) );
	}
	bool operator()( const CVec2 &a, const CVec2 &b ) const { return a.x < b.x || (a.x == b.x && a.y < b.y); }
};
struct SVec2Equ
{
	enum { bucket_size = 4 };
	size_t operator()( const CVec2 &v ) const
	{
		return (size_t)( (int(v.x)<<16) | int(v.y) );
	}
	bool operator()( const CVec2 &v1, const CVec2 &v2 ) const
	{
		return fabs2( v1.x - v2.x, v1.y - v2.y ) < 0.0000001f;
	}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // __AI_HASH_FUNCS__


