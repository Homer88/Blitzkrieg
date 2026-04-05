#ifndef __SAMPLESOUNDS_H__
#define __SAMPLESOUNDS_H__

#include "fmod.hpp"
#include "SFX.h"

// ========================================================================
// Базовый класс для сэмпла (звукового ресурса)
// ========================================================================
class CSoundSample : public ISharedResource
{
    OBJECT_NORMAL_METHODS(CSoundSample);
    SHARED_RESOURCE_METHODS(nRefData.a, "Sound");

    FMOD::Sound* m_pSound;
    FMOD_MODE      m_nMode;
    bool           m_bLooped;
    float          m_fMinDistance;

    void Close() { if (m_pSound) m_pSound->release(); m_pSound = nullptr; }

public:
    CSoundSample() : m_pSound(nullptr), m_nMode(FMOD_2D), m_bLooped(false), m_fMinDistance(45.0f) {}
    ~CSoundSample() { Close(); }

    void SetSound(FMOD::Sound* pSound)
    {
        Close();
        m_pSound = pSound;
        if (m_pSound)
            m_pSound->set3DMinMaxDistance(m_fMinDistance, 1000000000.0f);
    }

    FMOD_MODE GetMode() const { return m_nMode; }
    bool IsLooped() const { return m_bLooped; }
    FMOD::Sound* GetInternalSound() { Load(); return m_pSound; }

    void Set3D(bool b3D) { m_nMode = b3D ? (FMOD_3D | FMOD_3D_WORLDRELATIVE) : FMOD_2D; }
    void SetLoop(bool bEnable);
    void SetMinDistance(float fMinDistance)
    {
        m_fMinDistance = fMinDistance;
        if (m_pSound)
            m_pSound->set3DMinMaxDistance(fMinDistance, 1000000000.0f);
    }

    void STDCALL SwapData(ISharedResource* pResource) override;
    void STDCALL ClearInternalContainer() override {}
    bool STDCALL Load(const bool bPreLoad = false) override;
};

// ========================================================================
// Базовый класс для звука (воспроизводимый экземпляр)
// ========================================================================
class CBaseSound : public ISound
{
    DECLARE_SERIALIZE;

protected:
    CPtr<CSoundSample> m_pSample;
    FMOD::Channel* m_pChannel;
    float              m_fCurrentVolume;
    float              m_fCurrentPan;

public:
    CBaseSound() : m_pChannel(nullptr), m_fCurrentVolume(1.0f), m_fCurrentPan(0.0f) {}
    virtual ~CBaseSound() { Stop(); }

    void SetSample(CSoundSample* pSample) { m_pSample = pSample; }
    CSoundSample* GetSample() const { return m_pSample; }
    FMOD::Channel* GetChannel() const { return m_pChannel; }
    void SetChannel(FMOD::Channel* pChannel) { m_pChannel = pChannel; }

    void Stop()
    {
        if (m_pChannel)
        {
            m_pChannel->stop();
            m_pChannel = nullptr;
        }
    }
    bool IsPlaying()
    {
        if (!m_pChannel) return false;
        bool playing = false;
        m_pChannel->isPlaying(&playing);
        return playing;
    }

    // Реализация ISound
    virtual int STDCALL Visit(ISFXVisitor* pVisitor) override = 0;
    virtual void STDCALL SetMinDistance(float fDistance) override { if (m_pSample) m_pSample->SetMinDistance(fDistance); }
    virtual void STDCALL SetLooping(bool bEnable, int nStart = -1, int nEnd = -1) override;
    virtual unsigned int STDCALL GetLenght() override;
    virtual unsigned int STDCALL GetSampleRate() override;

    virtual void STDCALL SetVolume(float nVolume) override
    {
        m_fCurrentVolume = nVolume;
        if (m_pChannel) m_pChannel->setVolume(nVolume);
    }
    virtual float STDCALL GetVolume() const override { return m_fCurrentVolume; }

    virtual void STDCALL SetPan(float nPan) override
    {
        m_fCurrentPan = nPan;
        if (m_pChannel) m_pChannel->setPan(nPan);
    }
    virtual float STDCALL GetPan() const override { return m_fCurrentPan; }

    // Метод воспроизведения (чисто виртуальный, реализуется в наследниках)
    virtual int STDCALL Play() = 0;
};

// ========================================================================
// 2D звук
// ========================================================================
class CSound2D : public CBaseSound
{
    OBJECT_NORMAL_METHODS(CSound2D);
    DECLARE_SERIALIZE;

    float m_fVolume;
    float m_fPan;

public:
    CSound2D() : m_fVolume(1.0f), m_fPan(0.0f) {}
    virtual ~CSound2D() {}

    virtual int STDCALL Visit(ISFXVisitor* pVisitor) override;
    virtual int STDCALL Play() override;   // переопределяем
    virtual void STDCALL SetPosition(const CVec3& vPos3) override {}
    virtual const CVec3 STDCALL GetPosition() override { return VNULL3; }
    virtual void STDCALL SetVolume(float fVolume) override { m_fVolume = fVolume; CBaseSound::SetVolume(fVolume); }
    virtual float STDCALL GetVolume() const override { return m_fVolume; }
    virtual void STDCALL SetPan(float fPan) override { m_fPan = fPan; CBaseSound::SetPan(fPan); }
    virtual float STDCALL GetPan() const override { return m_fPan; }
};

// ========================================================================
// 3D звук
// ========================================================================
class CSound3D : public CBaseSound
{
    OBJECT_NORMAL_METHODS(CSound3D);
    DECLARE_SERIALIZE;

    CVec3 m_vPos;
    bool  m_bDopplerFlag;
    NTimer::STime m_lastUpdateTime;
    CVec3 m_vLastPos;

public:
    CSound3D() : m_bDopplerFlag(false), m_lastUpdateTime(0), m_vLastPos(VNULL3) {}
    virtual ~CSound3D() {}

    virtual int STDCALL Visit(ISFXVisitor* pVisitor) override;
    virtual int STDCALL Play() override;   // переопределяем
    void STDCALL SetDopplerFlag(bool bDoppler) { m_bDopplerFlag = bDoppler; }
    virtual void STDCALL SetPosition(const CVec3& vPos3) override;
    virtual const CVec3 STDCALL GetPosition() override { return m_vPos; }
};

#endif // __SAMPLESOUNDS_H__