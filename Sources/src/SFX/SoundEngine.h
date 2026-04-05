#ifndef __SOUNDENGINE_H__
#define __SOUNDENGINE_H__

#include "SampleSounds.h"
#include "StreamFadeOff.h"
#include "fmod.hpp"

// Глобальный объект FMOD (определён в SoundEngine.cpp)
extern FMOD::System* g_pFMODSystem;

typedef std::hash_map<ISound*, FMOD::Channel*> CSoundChannelMap;
typedef std::hash_map<FMOD::Channel*, CPtr<ISound> > CChannelSoundMap;

class CSoundEngine : public ISFX
{
    OBJECT_NORMAL_METHODS(CSoundEngine);
    DECLARE_SERIALIZE;

    struct SDriverInfo
    {
        std::string szDriverName;
        bool isHardware3DAccelerated;
        bool supportEAXReverb;
        bool supportA3DOcclusions;
        bool supportA3DReflections;
        bool supportReverb;
    };
    struct SMelodyInfo
    {
        DECLARE_SERIALIZE;
    public:
        std::string szName;
        bool bLooped;
        void Clear() { szName.clear(); }
        bool IsValid() const { return !szName.empty(); }
    };

    typedef std::vector<SDriverInfo> CDriversInfo;
    CDriversInfo drivers;

    NTimer::STime timeLastUpdate;

    SMelodyInfo curMelody;
    SMelodyInfo nextMelody;
    FMOD::Sound* pStreamingSound;
    FMOD::Channel* nStreamingChannel;
    NTimer::STime timeStreamFinished;

    CSoundChannelMap channelsMap;   // sound -> channel
    CChannelSoundMap soundsMap;     // channel -> sound

    float fListenerDistance;
    CVec3 vLastListenerPos;
    bool bInited;
    bool bEnableSFX;
    bool bEnableStreaming;
    bool bSoundCardPresent;
    bool bPaused;
    bool bStreamingPaused;

    BYTE cSFXMasterVolume;
    BYTE cStreamMasterVolume;
    float fStreamCurrentVolume;
    bool bStreamPlaying;

    CStreamFadeOff streamFadeOff;

    void ClearChannels();
    bool SearchDevices();
    void CloseStreaming();
    void ReEnableSounds();

    CSoundEngine();
    virtual ~CSoundEngine() { Done(); }

    void UpdateCameraPos(const CVec3& vPos);

public:
    // internal-use service functions
    bool PlayNextMelody();
    void NotifyMelodyFinished();
    void MapSound(ISound* pSound, FMOD::Channel* pChannel);

    virtual BYTE STDCALL GetSFXMasterVolume() const { return cSFXMasterVolume; }
    virtual BYTE STDCALL GetStreamMasterVolume() const { return cStreamMasterVolume; }

    virtual IRefCount* STDCALL QI(int nInterfaceTypeID);
    virtual bool STDCALL IsInitialized();
    virtual bool STDCALL Init(HWND hWnd, int nDriver, ESFXOutputType output, int nMixRate, int nMaxChannels);
    virtual void STDCALL Done();

    virtual void STDCALL EnableSFX(bool bEnable) { bEnableSFX = bEnable; ReEnableSounds(); }
    virtual void STDCALL EnableStreaming(bool bEnable) { bEnableStreaming = bEnable; ReEnableSounds(); }
    virtual bool STDCALL IsSFXEnabled() const { return bEnableSFX && bSoundCardPresent; }
    virtual bool STDCALL IsStreamingEnabled() const { return bEnableStreaming && bSoundCardPresent; }

    virtual void STDCALL SetDistanceFactor(float fFactor);
    virtual void STDCALL SetRolloffFactor(float fFactor);
    virtual void STDCALL SetSFXMasterVolume(float fVolume);
    virtual void STDCALL SetStreamMasterVolume(float fVolume);

    virtual void STDCALL PlayStream(const char* pszFileName, bool bLooped = false, const unsigned int nTimeToFadePrevious = 0);
    virtual void STDCALL StopStream(const unsigned int nTimeToFade = 0);
    virtual bool STDCALL IsStreamPlaying() const;
    virtual void STDCALL SetStreamVolume(const float fVolume);
    virtual float STDCALL GetStreamVolume() const;

    virtual int STDCALL PlaySample(ISound* pSound, bool bLooped = false, unsigned int nStartPos = 0);
    virtual void STDCALL StopSample(ISound* pSound);
    virtual void STDCALL UpdateSample(ISound* pSound);
    virtual void STDCALL StopChannel(int nChannel);  // nChannel не используется, оставлен для совместимости

    virtual void STDCALL Update(ICamera* pCamera);
    virtual bool STDCALL Pause(bool bPause);
    virtual bool STDCALL PauseStreaming(bool bPause);
    virtual bool STDCALL IsPaused();
    virtual bool STDCALL IsPlaying(ISound* pSound);

    virtual unsigned int STDCALL GetCurrentPosition(ISound* pSound);
    virtual void STDCALL SetCurrentPosition(ISound* pSound, unsigned int pos);
};

#endif // __SOUNDENGINE_H__