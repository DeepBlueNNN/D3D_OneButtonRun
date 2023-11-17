#include "framework.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(string name)
	:Model(name)
{
	m_frameBuffer = new FrameBuffer();
	SetShader(L"Model/ModelAnimation.hlsl");
}

ModelAnimator::~ModelAnimator()
{
}

void ModelAnimator::Update()
{
	UpdateFrame();
	UpdateWorld();
}

void ModelAnimator::Render()
{
	if (m_texture == nullptr)
		CreateTexture();

	m_frameBuffer->SetVS(3);
	DC->VSSetShaderResources(0, 1, &m_srv);

	Model::Render();
}

void ModelAnimator::GUIRender()
{
	int clip = m_frameBuffer->Get().cur.clip;
	ImGui::SliderInt("Clip", &m_frameBuffer->Get().cur.clip, 0, m_clips.size() - 1);
	ImGui::SliderInt("Frame", (int*)&m_frameBuffer->Get().cur.curFrame, 0, m_clips[clip]->m_frameCount - 1);

	Model::GUIRender();
}

void ModelAnimator::ReadClip(string clipName, UINT clipNum)
{
	string path = "Assets/Clips/" + m_name + "/" + clipName + to_string(clipNum) + ".clip";

	BinaryReader reader(path);

	ModelClip* clip = new ModelClip();
	clip->m_name      = reader.String();
	clip->m_frameCount = reader.UInt();
	clip->m_tickPerSecond = reader.Float();
	
	UINT boneCount = reader.UInt();

	for (UINT i = 0; i < boneCount; i++)
	{
		KeyFrame* keyFrame = new KeyFrame();
		keyFrame->boneName = reader.String();

		UINT size = reader.UInt();
		if (size)
		{
			keyFrame->transforms.resize(size);
			void* ptr = (void*)keyFrame->transforms.data();
			reader.Byte(&ptr, sizeof(KeyTransform)*size);
		}
		clip->m_keyFrames[keyFrame->boneName] = keyFrame;
	}

	m_clips.push_back(clip);
}

void ModelAnimator::PlayClip(int clip, float scale, float takeTime)
{
	m_isPlay = true;

	m_frameBuffer->Get().next.clip = clip;
	m_frameBuffer->Get().next.scale = scale;
	m_frameBuffer->Get().takeTime = takeTime;
}

void ModelAnimator::CreateTexture()
{
	UINT clipCount = m_clips.size();

	m_clipTransforms = new ClipTransform[clipCount];
	m_nodeTransforms = new ClipTransform[clipCount];

	// Transform 월드변환
	for (UINT i = 0; i < clipCount; i++)
		CreateClipTransform(i);

	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width     =   MAX_BONE *4;
	desc.Height    =   MAX_FRAME;
	desc.MipLevels = 1;
	desc.ArraySize = clipCount;
	desc.Format     = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.Usage     = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	UINT pitchSize = MAX_BONE * sizeof(Matrix);
	UINT pageSize = pitchSize * MAX_FRAME;

	// SubResourceData에 trasnform 넣기
	// 1) virtual Memory Alloccation : 가상메모리 할당
	void* p = VirtualAlloc(nullptr, pageSize * clipCount, MEM_RESERVE, PAGE_READWRITE);

	for (UINT i = 0; i < clipCount; i++)
	{
		UINT  start = i * pageSize;
		for (UINT y = 0; y < MAX_FRAME; y++)
		{
			// 2) Clip Transform 가상메모리 temp에 Copy하기
			void* temp = (BYTE*)p + pitchSize * y + start;
			VirtualAlloc(temp,pitchSize,MEM_COMMIT, PAGE_READWRITE);
			memcpy(temp, m_clipTransforms[i].transform[y], pitchSize);
		}
	}

	// 3) subResource에 넣기
	D3D11_SUBRESOURCE_DATA* subResouce = new D3D11_SUBRESOURCE_DATA[clipCount];

	for (UINT i = 0; i < clipCount; i++)
	{
		void* temp = (BYTE*)p + i * pageSize; // 순차적으로 주소값 temp에 저장
		subResouce[i].pSysMem = temp;
		subResouce[i].SysMemPitch = pitchSize;
		subResouce[i].SysMemSlicePitch = pageSize;
	}

	DEVICE->CreateTexture2D(&desc, subResouce, &m_texture);

	SAFE_DELETE_ARRAY(subResouce);

	// ShaderResourceView만들기
	D3D11_SHADER_RESOURCE_VIEW_DESC srcDesc = {};
	srcDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srcDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srcDesc.Texture2DArray.MipLevels = 1;
	srcDesc.Texture2DArray.ArraySize = clipCount;

	DEVICE->CreateShaderResourceView(m_texture, &srcDesc, &m_srv);
}

void ModelAnimator::CreateClipTransform(UINT index)
{
	ModelClip* clip = m_clips[index];

	for (UINT i = 0; i < clip->m_frameCount; i++)
	{
		// clip의 SRT 정보를 World로 변환(상대->절대)
		UINT nodeIndex = 0;
		for (NodeData node : m_nodes)
		{
			Matrix animation = XMMatrixIdentity();
			KeyFrame* frame = clip->GetKeyFrame(node.name); // heap,spine01,spine02
			if (frame != nullptr)
			{
				KeyTransform& transform = frame->transforms[i];

				animation = XMMatrixTransformation(XMVectorZero(),
					XMQuaternionIdentity(), Vector3(transform.scale),
					XMVectorZero(), XMLoadFloat4(&transform.rot),
					Vector3(transform.pos));
			}
			else
			{
				// 애니메이션에 관여하지 않는 node의 경우 기본 matrix로 들어감
				animation = XMMatrixIdentity();
			}

			Matrix parent;
			int    parentIndex = node.parent;   // -1인 경우는 root
			if (parentIndex < 0)
				parent = XMMatrixIdentity();
			else
				parent = m_nodeTransforms[index].transform[i][parentIndex];

			m_nodeTransforms[index].transform[i][nodeIndex] = animation * parent; // absolute 

			int boneIndex = -1;
			if (m_boneMap.count(node.name))
				boneIndex = m_boneMap[node.name];

			if (boneIndex >= 0)
			{
				Matrix offset = m_bones[boneIndex].offset;
				offset *= m_nodeTransforms[index].transform[i][nodeIndex];
				m_clipTransforms[index].transform[i][boneIndex] = offset; 
			}

			nodeIndex++;
		}
	}
}

void ModelAnimator::UpdateFrame()
{
	if (!m_isPlay) return;

	FrameBuffer::Data& frameData = m_frameBuffer->Get();
	{
		ModelClip* clip = m_clips[frameData.cur.clip];
		clip->m_playTime += frameData.cur.scale * DELTA;
		if (frameData.cur.time >= 1.0f)
		{
			frameData.cur.curFrame = (frameData.cur.curFrame + 1) % (clip->m_frameCount - 1);
			frameData.cur.time -= 1.0f;
		}
	}
	{
		if (frameData.next.clip < 0)
			return;

		ModelClip* clip = m_clips[frameData.next.clip];
		frameData.tweenTime += DELTA / frameData.takeTime;

		if (frameData.takeTime >= 1.0f)
		{
			frameData.cur = frameData.next;
			frameData.tweenTime = 0.0f;
			// 초기화
			frameData.next.clip = -1;
			frameData.next.curFrame = 0;
			frameData.next.time = 0.0f;

			return;
		}

		frameData.next.time += clip->m_tickPerSecond * frameData.next.scale * DELTA;

		if (frameData.next.time >= 1.0f)
		{
			frameData.next.curFrame = (frameData.next.curFrame + 1) % (clip->m_frameCount - 1);
			frameData.next.time -= 1.0f;
		}
	}
}
