#ifndef __PLANEPATHTEST_STUBS_H__
#define __PLANEPATHTEST_STUBS_H__
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Stub definitions for PlanePathTest standalone build
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Plane manuver IDs (not present in AIClassesID.h)
enum
{
	AI_PLANE_MANUVER_GENERIC = 0x20000000,
	AI_PLANE_MANUVER_GORKA,
};

// SConsts stubs - minimal definitions needed
struct SConsts
{
	static const int CELL_SIZE;
	static const int BIG_CELL_SIZE;
	static const int GENERAL_CELL_SIZE;
	static const int PLANE_MIN_HEIGHT;
};

// Define the constants
const int SConsts::CELL_SIZE = 64;
const int SConsts::BIG_CELL_SIZE = 256;
const int SConsts::GENERAL_CELL_SIZE = 128;
const int SConsts::PLANE_MIN_HEIGHT = 100;

// EMID_STEEPCLIMB definition
enum EManuverIDExtra
{
	EMID_STEEPCLIMB = 3,
};

// Random function stub
inline float Random( float minVal, float maxVal )
{
	return minVal + (rand() / float(RAND_MAX)) * (maxVal - minVal);
}

// CMineStaticObject stub
struct CMineStaticObject
{
	CVec3 vB2Pos;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // __PLANEPATHTEST_STUBS_H__
