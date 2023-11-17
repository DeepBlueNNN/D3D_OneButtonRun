#pragma once
// ClipData BinaryFile�� ����ִ� ����
// 
// BoneName : Scale(x,y,z), Rotation(x,y,z,w), Position(x,y,z) -> ���Matrix�� ��ϵ�
//  - �� ������ ���ᱸ���� Matrix������ ���� ����  : ex) Hips - transform   ->  Spine01 - transform(���)
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