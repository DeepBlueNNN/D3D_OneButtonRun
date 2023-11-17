#pragma once
// ClipData BinaryFile에 들어있는 정보
// 
// BoneName : Scale(x,y,z), Rotation(x,y,z,w), Position(x,y,z) -> 상대Matrix로 기록됨
//  - 각 본마다 연결구간의 Matrix정보가 아직 없다  : ex) Hips - transform   ->  Spine01 - transform(상대)
//

class ModelClip
{

private:
	friend class ModelAnimator;

public:
	ModelClip();
	~ModelClip();

public:
	KeyFrame* GetKeyFrame(string boneName);

private:
	string	m_name = "";

	UINT	m_frameCount = 0;
	float	m_tickPerSecond = 0.0f;
	float	m_duration = 0.0f;

	float	m_playTime = 0.0f;
	unordered_map<string, KeyFrame*> m_keyFrames;
};