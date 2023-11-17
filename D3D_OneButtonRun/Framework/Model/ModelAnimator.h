#pragma once
class ModelAnimator : public Model
{
protected:
    struct Frame
    {
        int    clip = 0;
        UINT   curFrame = 0;
        float  time = 0.0f;
        float  scale = 1.0f;
    };

    class FrameBuffer : public ConstBuffer
    {
    public:
        struct Data
        {
            float takeTime = 0.2f;
            float tweenTime = 0.0f;
            float runningTime = 0.0f;
            float padding = 0.0f;
            Frame cur, next;
        };

    public:
        FrameBuffer() : ConstBuffer(&m_data, sizeof(Data))
        {
        }

        Data& Get() { return m_data; }

    private:
        Data m_data;
    };

public:
	ModelAnimator(string name);
	~ModelAnimator();

public:
    void Update();
    void Render();
    void GUIRender();

    void ReadClip(string clipName, UINT clipNum = 0);
    void PlayClip(int clip, float scale = 1.0f, float takeTime = 0.2f);

protected:
    void CreateTexture();
    void CreateClipTransform(UINT index);

    void UpdateFrame();

protected:
    bool m_isPlay = true;

    vector<ModelClip*> m_clips;

    FrameBuffer* m_frameBuffer = nullptr;

    ClipTransform* m_clipTransforms = nullptr;
    ClipTransform* m_nodeTransforms = nullptr;

    ID3D11Texture2D* m_texture = nullptr;       // Clips의 데이터들
    ID3D11ShaderResourceView* m_srv = nullptr;  // Texture를 send
};