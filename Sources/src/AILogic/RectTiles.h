#ifndef __RECTTILES_H__
#define __RECTTILES_H__

#pragma ONCE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "AIHashFuncs.h"
#include "AICellsTiles.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef std::list<SVector> CTilesSet;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��� GetTilesCoveredByRectSides
class CTilesCollector
{
public:
	CTilesSet *pTiles;
	CTilesCollector(CTilesSet *pTiles) : pTiles(pTiles) { }
	bool operator() ( float x, float y );
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IsRectOnLockedTiles( const SRect &rect, const BYTE aiClass );
// ���������� tiles, ������� ��������� ������ ��������������
void GetTilesCoveredByQuadrangle( const CVec2 &v1, const CVec2 &v2, const CVec2 &v3, const CVec2 &v4, CTilesSet *pTiles );
// ���������� tiles, ������� ��������� ������ rect
void GetTilesCoveredByRect( const SRect &rect, CTilesSet *pTiles );
//���������� tiles, ������� ������������ �� ��������� ������� rect
void GetTilesCoveredByRectSides( const SRect &rect, CTilesSet *pTiles );
//�� �� �����, �� ����������� �������, ������� ����� � ������� dir �� ������ 
void GetTilesCoveredByRectSides( const SRect &rect, CTilesSet *pTiles, WORD dir );
//����� ��� ��������, ������� ����� � ������� dir �� ������ 
void GetTilesCoveredBySide( const SRect &rect, CTilesSet *pTiles, WORD dir );
// �������� �� ����� ��� rect ( ��� ���������� ������ � �� ������������ � ������� ), ���� pUnit �� �����������
bool IsMapFullyFree( const SRect &rect, interface IBasePathUnit *pUnit );
void GetTilesNextToRect( const SRect &rect, CTilesSet *pTiles );
//same as above, excludes tiles near given direction from center.
void GetTilesNextToRect( const SRect &rect, CTilesSet *pTiles, const WORD wDirExclude );
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator < ( const SVector &cell1, const SVector &cell2 );
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // __RECTTILES_H__


