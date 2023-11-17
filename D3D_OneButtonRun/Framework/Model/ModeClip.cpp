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

KeyFrame* ModelClip::GetKeyFrame(string boneName)
{
    if (m_keyFrames.count(boneName) == 0)
        return nullptr;

    return m_keyFrames[boneName];
}
