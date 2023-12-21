#include "framework.h"
#include "ModeClip.h"

ModelClip::ModelClip()
{
}

ModelClip::~ModelClip()
{
    for (auto a : m_keyFrames)
        SAFE_DELETE(a.second);

    m_keyFrames.clear();
}

void ModelClip::Init()
{
    m_eventIter = m_events.begin();
    m_playTime = 0.0f;
    m_duration = m_frameCount / m_tickPerSecond;
}

void ModelClip::Excute()
{
    if (m_events.empty()) return;
    if (m_eventIter == m_events.end()) return;

    float ratio = m_playTime / m_duration;

    if (m_eventIter->first > ratio) return;

    m_eventIter->second();
    m_eventIter++;
}

KeyFrame* ModelClip::GetKeyFrame(string boneName)
{
    if (m_keyFrames.count(boneName) == 0)
        return nullptr;

    return m_keyFrames[boneName];
}

void ModelClip::SetEvent(Event event, float timeRatio)
{
    if (m_events.count(timeRatio) > 0) return;

    m_events[timeRatio] = event;
}
