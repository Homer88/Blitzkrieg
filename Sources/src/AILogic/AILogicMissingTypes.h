// AILogicMissingTypes.h - Недостающие типы для совместимости с MSVC 2022
// Эти типы были потеряны при рефакторинге или не были объявлены явно

#ifndef __AILOGIC_MISSING_TYPES_H__
#define __AILOGIC_MISSING_TYPES_H__

#pragma ONCE

// ============================================================================
// IGun - интерфейс орудия (отсутствовал в проекте)
// ============================================================================
interface IGun : public IRefCount
{
	// Базовый интерфейс орудия
};

// ============================================================================
// IGunDescriptor - интерфейс дескриптора орудия (отсутствовал в проекте)
// ============================================================================
interface IGunDescriptor : public IRefCount
{
	virtual bool IsCorrect() const = 0;
	virtual interface IGun* GetGun() const = 0;
	virtual const int GetNGun() const = 0;
};

// ============================================================================
// SUpdatableObjectPtr - тип для placement new в Updater.cpp
// Это базовый тип для сериализации обновляемых объектов
// ============================================================================
struct SUpdatableObjectPtr
{
	IRefCount *pObj;
	int nFrameIndex;
	int nParam;
	int dbID;

	SUpdatableObjectPtr() : pObj( 0 ), nFrameIndex( -1 ), nParam( -1 ), dbID( -1 ) { }
};

// ============================================================================
// SUnitRPGInfo - псевдоним для SAINotifyRPGStats (используется в Updater.cpp)
// ============================================================================
struct SAINotifyRPGStats;
typedef SAINotifyRPGStats SUnitRPGInfo;

// ============================================================================
// SDiplomacyInfo - псевдоним для SAINotifyDiplomacy (используется в Updater.cpp)
// ============================================================================
struct SAINotifyDiplomacy;
typedef SAINotifyDiplomacy SDiplomacyInfo;

#endif // __AILOGIC_MISSING_TYPES_H__
