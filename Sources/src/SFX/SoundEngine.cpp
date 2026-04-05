#include "StdAfx.h"
#include "SoundEngine.h"
#include "SampleSounds.h"
#include "..\Scene\Scene.h"
#include "..\Formats\fmtTerrain.h"
#include "..\Misc\Win32Helper.h"

FMOD::System* g_pFMODSystem = nullptr;

// ========================================================================
// Конструктор / Деструктор
// ========================================================================
CSoundEngine::CSoundEngine()
    : bInited(false), pStreamingSound(nullptr), nStreamingChannel(nullptr),
    bPaused(false), bStreamingPaused(false),
    cSFXMasterVolume(255), cStreamMasterVolume(255), bEnableSFX(true), bEnableStreaming(true),
    timeLastUpdate(-1), timeStreamFinished(-1), fStreamCurrentVolume(1.0f),
    bStreamPlaying(false)
{
}

void CSoundEngine::Done()
{
    if (g_pFMODSystem)
    {
        g_pFMODSystem->close();
        g_pFMODSystem->release();
        g_pFMODSystem = nullptr;
    }
    drivers.clear();
    CloseStreaming();
    channelsMap.clear();
    soundsMap.clear();
}

// ========================================================================
// Инициализация FMOD
// ========================================================================
bool CSoundEngine::SearchDevices()
{
    // В FMOD Studio устройства перечисляются иначе
    // Для простоты пропустим детальное перечисление
    return true;
}

bool CSoundEngine::Init(HWND hWnd, int nDriver, ESFXOutputType output, int nMixRate, int nMaxChannels)
{
    // Создаём систему FMOD
    FMOD_RESULT res = FMOD::System_Create(&g_pFMODSystem);
    if (res != FMOD_OK) return false;

    // Устанавливаем вывод (здесь можно выбрать DSOUND, WASAPI и т.д.)
    FMOD_OUTPUTTYPE outType = FMOD_OUTPUTTYPE_DSOUND;
    switch (output)
    {
    case SFX_OUTPUT_NO: outType = FMOD_OUTPUTTYPE_NOSOUND; break;
    case SFX_OUTPUT_WINMM: outType = FMOD_OUTPUTTYPE_WINMM; break;
    case SFX_OUTPUT_DSOUND: outType = FMOD_OUTPUTTYPE_DSOUND; break;
    case SFX_OUTPUT_A3D: outType = FMOD_OUTPUTTYPE_DSOUND; break; // A3D не поддерживается
    }
    g_pFMODSystem->setOutput(outType);
    g_pFMODSystem->setSoftwareFormat(nMixRate, FMOD_SPEAKERMODE_STEREO, 0);
    g_pFMODSystem->setDSPBufferSize(1024, 4);

    // Инициализация
    res = g_pFMODSystem->init(nMaxChannels, FMOD_INIT_NORMAL, nullptr);
    if (res != FMOD_OK)
    {
        bSoundCardPresent = false;
        return true;  // звук отключён, но не ошибка
    }

    bSoundCardPresent = true;
    bInited = true;
    return true;
}

// ========================================================================
// Воспроизведение сэмпла
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
    int result = pBase->Play();   // Play вернёт 0 при успехе
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
    return result;  // 0 - успех, -1 - ошибка
}

void CSoundEngine::MapSound(ISound* pSound, FMOD::Channel* pChannel)
{
    channelsMap[pSound] = pChannel;
    soundsMap[pChannel] = pSound;
}

void CSoundEngine::StopSample(ISound* pSound)
{
    auto it = channelsMap.find(pSound);
    if (it != channelsMap.end())
    {
        it->second->stop();
        soundsMap.erase(it->second);
        channelsMap.erase(it);
    }
}

void CSoundEngine::UpdateSample(ISound* pSound)
{
    auto it = channelsMap.find(pSound);
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
    for (auto& pair : soundsMap)
    {
        bool playing = false;
        pair.first->isPlaying(&playing);
        if (!playing)
        {
            toErase.push_back(pair.first);
            pair.first->stop();
        }
    }
    for (auto& ch : toErase)
    {
        auto it = soundsMap.find(ch);
        if (it != soundsMap.end())
        {
            channelsMap.erase(it->second);
            soundsMap.erase(it);
        }
    }
}