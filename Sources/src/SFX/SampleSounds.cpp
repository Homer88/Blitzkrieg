#include "StdAfx.h"
#include "SampleSounds.h"
#include "SoundEngine.h"

extern FMOD::System* g_pFMODSystem;

// ========================================================================
// CSoundSample
// ========================================================================
bool CSoundSample::Load(const bool bPreLoad)
{
    if (m_pSound != nullptr || bPreLoad) return true;

    std::string szStreamName = GetSharedResourceFullName();
    CPtr<IDataStream> pStream = GetSingleton<IDataStorage>()->OpenStream(szStreamName.c_str(), STREAM_ACCESS_READ);
    if (pStream == nullptr) return false;

    int nSize = pStream->GetSize();
    std::vector<char> buffer(nSize);
    if (pStream->Read(&buffer[0], nSize) != nSize) return false;

    FMOD_CREATESOUNDEXINFO exinfo = { 0 };
    exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
    exinfo.length = nSize;

    FMOD_MODE mode = FMOD_OPENMEMORY_POINT | FMOD_CREATECOMPRESSEDSAMPLE | GetMode();
    if (m_bLooped) mode |= FMOD_LOOP_NORMAL;
    else mode |= FMOD_LOOP_OFF;

    FMOD::Sound* pSound = nullptr;
    FMOD_RESULT res = g_pFMODSystem->createSound(&buffer[0], mode, &exinfo, &pSound);
    if (res != FMOD_OK) return false;

    SetSound(pSound);
    return true;
}

void CSoundSample::SetLoop(bool bEnable)
{
    m_bLooped = bEnable;
    if (m_pSound)
    {
        FMOD_MODE mode;
        m_pSound->getMode(&mode);
        if (bEnable) mode |= FMOD_LOOP_NORMAL;
        else mode &= ~FMOD_LOOP_NORMAL;
        m_pSound->setMode(mode);
    }
}

void CSoundSample::SwapData(ISharedResource* pResource)
{
    CSoundSample* pRes = dynamic_cast<CSoundSample*>(pResource);
    NI_ASSERT_TF(pRes != 0, NStr::Format("shared resource is not a \"%s\"", typeid(*this).name()), return);
    std::swap(m_pSound, pRes->m_pSound);
    std::swap(m_nMode, pRes->m_nMode);
    std::swap(m_bLooped, pRes->m_bLooped);
    std::swap(m_fMinDistance, pRes->m_fMinDistance);
}

// ========================================================================
// CBaseSound
// ========================================================================
void CBaseSound::SetLooping(bool bEnable, int nStart, int nEnd)
{
    if (m_pSample && m_pSample->GetInternalSound())
    {
        FMOD::Sound* pSound = m_pSample->GetInternalSound();
        FMOD_MODE mode;
        pSound->getMode(&mode);
        if (bEnable) mode |= FMOD_LOOP_NORMAL;
        else mode &= ~FMOD_LOOP_NORMAL;
        pSound->setMode(mode);
        if (nStart >= 0 && nEnd >= 0)
            pSound->setLoopPoints(nStart, FMOD_TIMEUNIT_PCM, nEnd, FMOD_TIMEUNIT_PCM);
    }
}

unsigned int CBaseSound::GetLenght()
{
    if (!m_pSample) return 0;
    FMOD::Sound* pSound = m_pSample->GetInternalSound();
    if (!pSound) return 0;
    unsigned int length = 0;
    pSound->getLength(&length, FMOD_TIMEUNIT_PCM);
    return length;
}

unsigned int CBaseSound::GetSampleRate()
{
    if (!m_pSample) return 0;
    FMOD::Sound* pSound = m_pSample->GetInternalSound();
    if (!pSound) return 0;
    float freq = 0;
    int priority = 0;
    pSound->getDefaults(&freq, &priority);
    return static_cast<int>(freq);
}

// ========================================================================
// CSound2D
// ========================================================================
int CSound2D::Play()
{
    if (!m_pSample) return -1;
    FMOD::Sound* pSound = m_pSample->GetInternalSound();
    if (!pSound) return -1;

    FMOD::Channel* pChannel = nullptr;
    FMOD_RESULT res = g_pFMODSystem->playSound(pSound, nullptr, true, &pChannel);
    if (res != FMOD_OK) return -1;

    pChannel->setVolume(m_fVolume);
    pChannel->setPan(m_fPan);
    pChannel->setPaused(false);

    SetChannel(pChannel);
    return 0;
}

int CSound2D::Visit(ISFXVisitor* pVisitor)
{
    return pVisitor->VisitSound2D(this);
}

// ========================================================================
// CSound3D
// ========================================================================
int CSound3D::Play()
{
    if (!m_pSample) return -1;
    FMOD::Sound* pSound = m_pSample->GetInternalSound();
    if (!pSound) return -1;

    FMOD::Channel* pChannel = nullptr;
    FMOD_RESULT res = g_pFMODSystem->playSound(pSound, nullptr, true, &pChannel);
    if (res != FMOD_OK) return -1;

    FMOD_VECTOR pos = { m_vPos.x, m_vPos.z, m_vPos.y };
    pChannel->set3DAttributes(&pos, nullptr);
    pChannel->setPaused(false);

    SetChannel(pChannel);
    return 0;
}

void CSound3D::SetPosition(const CVec3& vPos3)
{
    m_vPos = vPos3;
    if (IsPlaying())
    {
        FMOD_VECTOR pos = { vPos3.x, vPos3.z, vPos3.y };
        m_pChannel->set3DAttributes(&pos, nullptr);
    }
}

int CSound3D::Visit(ISFXVisitor* pVisitor)
{
    return pVisitor->VisitSound3D(this, m_vPos);
}