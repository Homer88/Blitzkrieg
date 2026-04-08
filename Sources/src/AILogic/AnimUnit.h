#ifndef __ANIM_UNIT_H__
#define __ANIM_UNIT_H__
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma ONCE
#include "AIUnit.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
interface IAnimUnit : public IRefCount
{
	virtual void AnimationSet( int nAnimation ) = 0;
	virtual void Moved() = 0;
	virtual void Stopped() = 0;
	virtual void StopCurAnimation() = 0;

	virtual void Segment() = 0;

	virtual void Init( class CAIUnit *pOwner ) = 0;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CAnimUnit - конкретная реализация для CAIUnit (используется для BASIC_REGISTER_CLASS)
class CAnimUnit : public IAnimUnit
{
	OBJECT_COMPLETE_METHODS( CAnimUnit );

	CAIUnit *pOwner;
	bool bTechnics;
	int nCurAnimation;
	NTimer::STime timeOfFinishAnimation;

	struct SMovingState
	{
		enum EMovingState { EMS_STOPPED, EMS_MOVING, EMS_STOPPED_TO_MOVING, EMS_MOVING_TO_STOPPED };
		EMovingState state;
		NTimer::STime timeOfIntentionStart;
		SMovingState() : state( EMS_STOPPED ), timeOfIntentionStart( 0 ) { }
	};
	SMovingState movingState;

public:
	CAnimUnit() : pOwner( 0 ), bTechnics( false ), nCurAnimation( 0 ), timeOfFinishAnimation( 0 ) { }
	CAnimUnit( CAIUnit *_pOwner );
	virtual void Init( CAIUnit *_pOwner ) { pOwner = _pOwner; }

	virtual void AnimationSet( int nAnimation );
	virtual void AnimationSet( int nAnimation, int nLength );

	virtual void Moved();
	virtual void Stopped();

	virtual void Segment();

	virtual void StopCurAnimation() { }
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // __ANIM_UNIT_H__


