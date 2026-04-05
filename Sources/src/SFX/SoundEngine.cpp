#include "StdAfx.h"
#include "SoundEngine.h"
#include "SampleSounds.h"
// #include "..\Scene\Scene.h"
// #include "..\Formats\fmtTerrain.h"
#include "..\Misc\Win32Helper.h"
// #include "..\StreamIO\DataStorage.h"   // для IDataStorage - временно отключено
#include <mmsystem.h>                   // для timeGetTime

FMOD::System* g_pFMODSystem = 0;

// ========================================================================
// Конструктор / Деструктор / Done
// ========================================================================
CSoundEngine::CSoundEngine()
    : bInited(false), pStreamingSound(0), nStreamingChannel(0),
      bPaused(false), bStreamingPaused(false),
      cSFXMasterVolume(255), cStreamMasterVolume(255), bEnableSFX(true), bEnableStreaming(true),
      timeLastUpdate(-1), timeStreamFinished(-1), fStreamCurrentVolume(1.0f),
      bStreamPlaying(false)
{
    streamFadeOff.Init();
}

void CSoundEngine::Done()
{
    if (g_pFMODSystem)
    {
        g_pFMODSystem->close();
        g_pFMODSystem->release();
        g_pFMODSystem = 0;
    }
    drivers.clear();
    CloseStreaming();
    channelsMap.clear();
    soundsMap.clear();
}

// ========================================================================
// CloseStreaming – остановка и освобождение стриминг-звука
// ========================================================================
void CSoundEngine::CloseStreaming()
{
    if (pStreamingSound)
    {
        pStreamingSound->release();
        pStreamingSound = 0;
    }
    nStreamingChannel = 0;
    bStreamPlaying = false;
    curMelody.Clear();
    nextMelody.Clear();
    timeStreamFinished = -1;
}

// ========================================================================
// ReEnableSounds – перезапуск звуков после включения SFX
// ========================================================================
void CSoundEngine::ReEnableSounds()
{
    if (!bEnableSFX) return;
    // Обновить громкости всех активных каналов
    for (CSoundChannelMap::iterator it = channelsMap.begin(); it != channelsMap.end(); ++it)
    {
        ISound* pSound = it->first;
        FMOD::Channel* pChannel = it->second;
        if (pChannel)
            pChannel->setVolume(pSound->GetVolume() * (cSFXMasterVolume / 255.0f));
    }
    // Если стриминг включён и была мелодия – восстановить
    if (bEnableStreaming && curMelody.IsValid())
    {
        PlayStream(curMelody.szName.c_str(), curMelody.bLooped);
    }
}

// ========================================================================
// NotifyMelodyFinished – вызывается по окончании стрима
// ========================================================================
void CSoundEngine::NotifyMelodyFinished()
{
    if (!bEnableStreaming) return;
    if (nextMelody.IsValid())
    {
        PlayStream(nextMelody.szName.c_str(), nextMelody.bLooped);
        nextMelody.Clear();
    }
    else
    {
        bStreamPlaying = false;
        // timeStreamFinished = NTimer::GetTime(); // Временно отключено
    }
}

// ========================================================================
// PlayNextMelody – воспроизвести следующую мелодию (заглушка)
// ========================================================================
bool CSoundEngine::PlayNextMelody()
{
    if (!bEnableStreaming) return false;
    // Реализация может брать следующую мелодию из глобального плейлиста
    return false;
}

// ========================================================================
// SearchDevices (заглушка)
// ========================================================================
bool CSoundEngine::SearchDevices()
{
    return true;
}

// ========================================================================
// Init
// ========================================================================
bool CSoundEngine::Init(HWND hWnd, int nDriver, ESFXOutputType output, int nMixRate, int nMaxChannels)
{
    FMOD_RESULT res = FMOD::System_Create(&g_pFMODSystem);
    if (res != FMOD_OK) return false;

    FMOD_OUTPUTTYPE outType = FMOD_OUTPUTTYPE_DSOUND;
    switch (output)
    {
        case SFX_OUTPUT_NO: outType = FMOD_OUTPUTTYPE_NOSOUND; break;
        case SFX_OUTPUT_WINMM: outType = FMOD_OUTPUTTYPE_WINMM; break;
        case SFX_OUTPUT_DSOUND: outType = FMOD_OUTPUTTYPE_DSOUND; break;
        case SFX_OUTPUT_A3D: outType = FMOD_OUTPUTTYPE_DSOUND; break;
    }
    g_pFMODSystem->setOutput(outType);
    g_pFMODSystem->setSoftwareFormat(nMixRate, FMOD_SPEAKERMODE_STEREO, 0);
    g_pFMODSystem->setDSPBufferSize(1024, 4);

    res = g_pFMODSystem->init(nMaxChannels, FMOD_INIT_NORMAL, 0);
    if (res != FMOD_OK)
    {
        bSoundCardPresent = false;
        return true;
    }

    bSoundCardPresent = true;
    bInited = true;
    return true;
}

// ========================================================================
// PlaySample
// ========================================================================
int CSoundEngine::PlaySample(ISound* pSound, bool bLooped, unsigned int nStartPos)
{
    if (!pSound || !bEnableSFX) return -1;

    CSound2D* p2D = dynamic_cast<CSound2D*>(pSound);
    CSound3D* p3D = dynamic_cast<CSound3D*>(pSound);
    if (!p2D && !p3D) return -1;

    CBaseSound* pBase = static_cast<CBaseSound*>(pSound);
    if (!pBase->GetSample()) return -1;

    pBase->GetSample()->SetLoop(bLooped);
    int result = pBase->Play();
    if (result == 0)
    {
        FMOD::Channel* pChannel = pBase->GetChannel();
        if (pChannel)
        {
            pChannel->setPosition(nStartPos, FMOD_TIMEUNIT_PCM);
            pChannel->setPaused(false);
            MapSound(pSound, pChannel);
        }
    }
    return result;
}

void CSoundEngine::MapSound(ISound* pSound, FMOD::Channel* pChannel)
{
    channelsMap[pSound] = pChannel;
    soundsMap[pChannel] = pSound;
}

void CSoundEngine::StopSample(ISound* pSound)
{
    CSoundChannelMap::iterator it = channelsMap.find(pSound);
    if (it != channelsMap.end())
    {
        it->second->stop();
        soundsMap.erase(it->second);
        channelsMap.erase(it);
    }
}

void CSoundEngine::UpdateSample(ISound* pSound)
{
    CSoundChannelMap::iterator it = channelsMap.find(pSound);
    if (it != channelsMap.end())
    {
        FMOD::Channel* pChannel = it->second;
        pChannel->setVolume(pSound->GetVolume() * (cSFXMasterVolume / 255.0f));
        pChannel->setPan(pSound->GetPan());
    }
}

void CSoundEngine::ClearChannels()
{
    if (bPaused) return;

    std::list<FMOD::Channel*> toErase;
    for (CChannelSoundMap::iterator it = soundsMap.begin(); it != soundsMap.end(); ++it)
    {
        bool playing = false;
        it->first->isPlaying(&playing);
        if (!playing)
        {
            toErase.push_back(it->first);
            it->first->stop();
        }
    }
    for (std::list<FMOD::Channel*>::iterator it = toErase.begin(); it != toErase.end(); ++it)
    {
        CChannelSoundMap::iterator itMap = soundsMap.find(*it);
        if (itMap != soundsMap.end())
        {
            channelsMap.erase(itMap->second);
            soundsMap.erase(itMap);
        }
    }
}

// ========================================================================
// QI
// ========================================================================
IRefCount* STDCALL CSoundEngine::QI(int nInterfaceTypeID)
{
    if (nInterfaceTypeID == SFX_SFX)
        return static_cast<ISFX*>(this);
    return 0;
}

// ========================================================================
// IsInitialized
// ========================================================================
bool STDCALL CSoundEngine::IsInitialized()
{
    return bInited && g_pFMODSystem != 0;
}

// ========================================================================
// SetDistanceFactor
// ========================================================================
void STDCALL CSoundEngine::SetDistanceFactor(float fFactor)
{
    if (g_pFMODSystem)
        g_pFMODSystem->set3DSettings(1.0f, fFactor, 1.0f);
}

// ========================================================================
// SetRolloffFactor
// ========================================================================
void STDCALL CSoundEngine::SetRolloffFactor(float fFactor)
{
    if (g_pFMODSystem)
        g_pFMODSystem->set3DSettings(1.0f, 1.0f, fFactor);
}

// ========================================================================
// SetSFXMasterVolume
// ========================================================================
void STDCALL CSoundEngine::SetSFXMasterVolume(float fVolume)
{
    cSFXMasterVolume = (BYTE)(fVolume * 255.0f);
    for (CSoundChannelMap::iterator it = channelsMap.begin(); it != channelsMap.end(); ++it)
    {
        ISound* pSound = it->first;
        FMOD::Channel* pChannel = it->second;
        if (pChannel)
            pChannel->setVolume(pSound->GetVolume() * (cSFXMasterVolume / 255.0f));
    }
}

// ========================================================================
// SetStreamMasterVolume
// ========================================================================
void STDCALL CSoundEngine::SetStreamMasterVolume(float fVolume)
{
    cStreamMasterVolume = (BYTE)(fVolume * 255.0f);
    SetStreamVolume(fStreamCurrentVolume);
}

// ========================================================================
// PlayStream
// ========================================================================
void STDCALL CSoundEngine::PlayStream(const char* pszFileName, bool bLooped, const unsigned int nTimeToFadePrevious)
{
    if (!bEnableStreaming || !pszFileName || !pszFileName[0]) return;

    if (IsStreamPlaying() && nTimeToFadePrevious > 0)
    {
        streamFadeOff.Fade(nTimeToFadePrevious);
        nextMelody.szName = pszFileName;
        nextMelody.bLooped = bLooped;
        return;
    }

    StopStream(0);

    // Открыть файл через IDataStorage
    CPtr<IDataStream> pStream = GetSingleton<IDataStorage>()->OpenStream(pszFileName, STREAM_ACCESS_READ);
    if (!pStream) return;

    int nSize = pStream->GetSize();
    std::vector<char> buffer(nSize);
    if (pStream->Read(&buffer[0], nSize) != nSize) return;

    FMOD_CREATESOUNDEXINFO exinfo = {0};
    exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    exinfo.length = nSize;

    FMOD_MODE mode = FMOD_OPENMEMORY_POINT | FMOD_CREATESTREAM | FMOD_2D;
    if (bLooped) mode |= FMOD_LOOP_NORMAL;

    FMOD::Sound* pNewSound = 0;
    FMOD_RESULT res = g_pFMODSystem->createSound(&buffer[0], mode, &exinfo, &pNewSound);
    if (res != FMOD_OK) return;

    FMOD::Channel* pChannel = 0;
    res = g_pFMODSystem->playSound(pNewSound, 0, true, &pChannel);
    if (res != FMOD_OK)
    {
        pNewSound->release();
        return;
    }

    CloseStreaming();

    pStreamingSound = pNewSound;
    nStreamingChannel = pChannel;
    curMelody.szName = pszFileName;
    curMelody.bLooped = bLooped;
    bStreamPlaying = true;
    timeStreamFinished = -1;

    SetStreamVolume(fStreamCurrentVolume);
    pChannel->setPaused(false);
}

// ========================================================================
// StopStream
// ========================================================================
void STDCALL CSoundEngine::StopStream(const unsigned int nTimeToFade)
{
    if (!pStreamingSound) return;

    if (nTimeToFade > 0)
    {
        streamFadeOff.Fade(nTimeToFade);
        return;
    }

    if (nStreamingChannel)
    {
        nStreamingChannel->stop();
        nStreamingChannel = 0;
    }
    CloseStreaming();
}

// ========================================================================
// IsStreamPlaying
// ========================================================================
bool STDCALL CSoundEngine::IsStreamPlaying() const
{
    if (!bEnableStreaming || !nStreamingChannel) return false;
    bool playing = false;
    nStreamingChannel->isPlaying(&playing);
    return playing || streamFadeOff.IsFading();
}

// ========================================================================
// SetStreamVolume
// ========================================================================
void STDCALL CSoundEngine::SetStreamVolume(const float fVolume)
{
    fStreamCurrentVolume = fVolume;
    if (nStreamingChannel)
    {
        float finalVol = fVolume * (cStreamMasterVolume / 255.0f);
        nStreamingChannel->setVolume(finalVol);
    }
}

// ========================================================================
// GetStreamVolume
// ========================================================================
float STDCALL CSoundEngine::GetStreamVolume() const
{
    return fStreamCurrentVolume;
}

// ========================================================================
// StopChannel – заглушка
// ========================================================================
void STDCALL CSoundEngine::StopChannel(int /*nChannel*/)
{
    // не используется
}

// ========================================================================
// UpdateCameraPos – вспомогательная (пустая)
// ========================================================================
void CSoundEngine::UpdateCameraPos(const CVec3& vPos)
{
    // не используется, всё делается в Update через FMOD
}

// ========================================================================
// Update – обновление 3D звуков и слушателя
// ========================================================================
void STDCALL CSoundEngine::Update(ICamera* pCamera)
{
    if (!bInited || !g_pFMODSystem) return;

    if (pCamera)
    {
        // TODO: Реализовать обновление 3D слушателя
        // CVec3 vPos = pCamera->GetWorldPos();
        // CVec3 vForward = pCamera->GetWorldDir();
        // CVec3 vUp = pCamera->GetWorldUp();
        //
        // FMOD_VECTOR pos = { vPos.x, vPos.z, vPos.y };
        // FMOD_VECTOR forward = { vForward.x, vForward.z, vForward.y };
        // FMOD_VECTOR up = { vUp.x, vUp.z, vUp.y };
        // g_pFMODSystem->set3DListenerAttributes(0, &pos, 0, &forward, &up);
    }

    g_pFMODSystem->update();
    ClearChannels();

    if (bStreamPlaying && nStreamingChannel)
    {
        bool playing = false;
        nStreamingChannel->isPlaying(&playing);
        if (!playing && !streamFadeOff.IsFading())
        {
            NotifyMelodyFinished();
        }
    }
}

// ========================================================================
// Pause / PauseStreaming / IsPaused
// ========================================================================
bool STDCALL CSoundEngine::Pause(bool bPause)
{
    if (!bInited) return false;
    bPaused = bPause;
    for (CSoundChannelMap::iterator it = channelsMap.begin(); it != channelsMap.end(); ++it)
    {
        if (it->second)
            it->second->setPaused(bPause);
    }
    return true;
}

bool STDCALL CSoundEngine::PauseStreaming(bool bPause)
{
    if (!bInited || !nStreamingChannel) return false;
    bStreamingPaused = bPause;
    nStreamingChannel->setPaused(bPause);
    return true;
}

bool STDCALL CSoundEngine::IsPaused()
{
    return bPaused;
}

// ========================================================================
// IsPlaying
// ========================================================================
bool STDCALL CSoundEngine::IsPlaying(ISound* pSound)
{
    CSoundChannelMap::iterator it = channelsMap.find(pSound);
    if (it == channelsMap.end()) return false;
    bool playing = false;
    it->second->isPlaying(&playing);
    return playing;
}

// ========================================================================
// GetCurrentPosition / SetCurrentPosition
// ========================================================================
unsigned int STDCALL CSoundEngine::GetCurrentPosition(ISound* pSound)
{
    CSoundChannelMap::iterator it = channelsMap.find(pSound);
    if (it == channelsMap.end()) return 0;
    unsigned int pos = 0;
    it->second->getPosition(&pos, FMOD_TIMEUNIT_PCM);
    return pos;
}

void STDCALL CSoundEngine::SetCurrentPosition(ISound* pSound, unsigned int pos)
{
    CSoundChannelMap::iterator it = channelsMap.find(pSound);
    if (it != channelsMap.end())
        it->second->setPosition(pos, FMOD_TIMEUNIT_PCM);
}

// ========================================================================
// Сериализация SMelodyInfo
// ========================================================================
int CSoundEngine::SMelodyInfo::operator&(IStructureSaver &ss)
{
    CSaverAccessor saver(&ss);
    saver.Add(1, &szName);
    saver.Add(2, &bLooped);
    return 0;
}

// ========================================================================
// Сериализация CSoundEngine
// ========================================================================
int CSoundEngine::operator&(IStructureSaver &ss)
{
    CSaverAccessor saver(&ss);
    saver.Add(1, &bInited);
    saver.Add(2, &bEnableSFX);
    saver.Add(3, &bEnableStreaming);
    saver.Add(4, &bSoundCardPresent);
    saver.Add(5, &cSFXMasterVolume);
    saver.Add(6, &cStreamMasterVolume);
    saver.Add(7, &curMelody);
    saver.Add(8, &nextMelody);
    saver.Add(9, &streamFadeOff);
    return 0;
}