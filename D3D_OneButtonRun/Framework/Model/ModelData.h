#pragma once
typedef VertexUVNormalTangentBlend ModelVertex;

struct  MeshData
{
    string  name = "";
    UINT    matrialIndex =0;
    vector<ModelVertex> vertices;
    vector<UINT>        indices;

};

struct asBlendWeight
{
	Float4 Indices = Float4(0, 0, 0, 0);
	Float4 Weights = Float4(0, 0, 0, 0);

	void Set(UINT index, UINT boneIndex, float weight)
	{
		float i = (float)boneIndex;
		float w = weight;

		switch (index)
		{
		case 0: Indices.x = i; Weights.x = w; break;
		case 1: Indices.y = i; Weights.y = w; break;
		case 2: Indices.z = i; Weights.z = w; break;
		case 3: Indices.w = i; Weights.w = w; break;
		}
	}
};

struct SortVertexWeights
{
    UINT   index = 0;
    float  weight = 0.0f;
};

struct VertexWeights
{
    UINT  indices[4] = { 0, };
    float weights[4] = { 0.0f, };
    vector<SortVertexWeights>  sWeights;

    void  Add(const UINT& index, const float& weight)
    {
        if (weight < 0.000001f)
            return;

        SortVertexWeights s;
        s.index = index;
        s.weight = weight;

        for (UINT i = 0; i < sWeights.size(); i++)
        {
            if (sWeights[i].weight < weight)
            {
                sWeights.insert(sWeights.begin()+i,s);
                break;
            }
        }
        if(sWeights.size()==0)
            sWeights.push_back(s);
    }
    void Normalize()
    {
        float sum = 0.0f;
      
        if (sWeights.size() > 0)
        {
            for (UINT i = 0; i < 4; i++)
            {
                if (i <= (sWeights.size() - 1))
                {
                    indices[i] = sWeights[i].index;
                    weights[i] = sWeights[i].weight;
                }
            }
           sWeights.clear();
        }

        for (UINT i = 0; i < 4; i++)
        {
            sum = sum + weights[i];
        }
        for (UINT i = 0; i < 4; i++)
        {
            weights[i] = weights[i] / sum;
        }
    }
};

struct  BoneData
{
    int    index = 0;
    string name = "";
    Matrix offset = XMMatrixIdentity();
};

struct  NodeData
{
    int    index =0;
    string name = "";
    int    parent = 0;
    Matrix transform = XMMatrixIdentity();
};

struct KeyTransform
{
    Float3 scale = Float3(0.0f, 0.0f, 0.0f);
    Float4 rot = Float4(0.0f, 0.0f, 0.0f, 0.0f);
    Float3 pos = Float3(0.0f, 0.0f, 0.0f);
};

struct KeyFrame
{
    string boneName = "";
    vector<KeyTransform> transforms;
};
struct ClipNode
{
    aiString name;
    vector<KeyTransform> transforms;
};

struct Clip
{
    string name = "";
    UINT   frameCount = 0;    // 믹사모에서 30frame 데이타로 추출
    float  tickPerSecond = 0.0f;
    vector<KeyFrame*> KeyFrame;
};

struct ClipTransform
{
    Matrix transform[MAX_BONE][MAX_FRAME] = {};
};