#ifndef __SMOKIN_PARTICLESOURCEDATA_H__
#define __SMOKIN_PARTICLESOURCEDATA_H__
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Track.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct SSmokinParticleSourceData : public CTRefCount<ISharedResource>
{
	OBJECT_SERVICE_METHODS( SSmokinParticleSourceData );
	SHARED_RESOURCE_METHODS( nRefData, "ComplexParticleSource.Data" );
public:
	SSmokinParticleSourceData();

	bool bComplexParticleSource;					//��� ���������, ���� true, complex particle source
	// ��������� ����������
	int nLifeTime;												// ����� ����� ����� ���������
	float fGravity;												// �������� ���������� (�������� ��������� � ������ �� �����)
	CVec3 vWind;                          // �����
	CVec3 vDirection;                     // ����������� ���������
	CTrack trackGenerateArea;							// ������ ������� �� ������� �������� �������� 
	CTrack trackDensity;									// �-�� ��������� ��������� � ��. ������� 
	CTrack trackBeginSpeed;								// ��������� �������� ������� ���  ������ 
	CTrack trackBeginSpeedRandomizer;     // �� ������������
	CTrack trackBeginAngleRandomizer;     // ������������ ���� ������
	// ��������� ����� �������
	CTrack trackSpeed;                    // �����. ��������
	CTrack trackSpeedRnd;                 // ������������ �����. ��������
	CTrack trackWeight;										// ����� �������� (�������� ��������� � ���������� ����� �� �����)
	int nAreaType;                        // ��� ������� ��� ��������� ���������
	float fRadialWind;                    // ���� ����������� �����
	std::string szParticleEffectName;     // �������� ��������������� �������
	CTrack trackIntegralMass;             // ������������� �� g*m(t), ���������� �� ����������� �������� (��� ��� �������)
	float fDensityCoeff;                  // ����������� �� ���������, ������� �� ���������
	int nUpdateStep;                      // ���������� ����������� ����� update'���
	//

	virtual void STDCALL SwapData( ISharedResource *pResource );
	// internal container clearing
	virtual void STDCALL ClearInternalContainer() {  }
	virtual bool STDCALL Load( const bool bPreLoad = false );
	//
	virtual int STDCALL operator&( IStructureSaver &ss );
	virtual int STDCALL operator&( IDataTree &ss );
	virtual void STDCALL InitIntegrals();
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif 
