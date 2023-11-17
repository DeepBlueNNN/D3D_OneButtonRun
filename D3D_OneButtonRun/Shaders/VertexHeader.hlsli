// Vertex
cbuffer WorldBuffer : register(b0)
{
    matrix world;
}
cbuffer ViewBuffer : register(b1)
{
    matrix view;
    matrix invView;  // 원래 ViewMatrix Inverse(카메라위치)
}
cbuffer ProjectionBuffer : register(b2)
{
    matrix projection;
}

struct Frame
{
    int   clip;
    uint  curFrame;
    float time;
    float speed;
};

struct Motion
{
    float takeTime;
    float tweenTime;
    float runningTime;
    float padding;
    
    Frame cur;
    Frame next;
};

cbuffer CFrameBuffer : register(b3)
{
    Motion motion;
}


// Texture2D Array  :  ModelAnimator에서 생성
//////////////////////////////////////// Clip(Dancing)
//         srt, srt,srt  : bone갯수
// Frame1 
// ~~
// Frame30
///////////////////////////////////////
Texture2DArray transformMap : register(t0);

//Vertex Layout
struct Vertex
{
    float4 pos : POSITION;
};

struct VertexUV
{
    float4 pos : POSITION;
    float2 uv : UV;
};

struct VertexColor
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct VertexUVNormal
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
};

struct VertexUVNormalTangent
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VertexUVNormalTangentAlpha
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 alpha : ALPHA;
};

struct VertexUVNormalTangentBlend
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 indices : BLENDINDICES;    // 추가
    float4 weights : BLENDWEIAGHTS;   // 추가
};

struct VertexInstancing
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 indices : BLENDINDICES;
    float4 weights : BLENDWEIGHTS;
    
    matrix transform : INSTANCE_TRANSFORM;
    int    index     : INSTANCE_INDEX;
};

matrix SkinWorld(float4 indices, float4 weights)
{
    matrix transform = 0;
    matrix cur,next;
    matrix curAnim, nextAnim;
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    for (int i = 0; i < 4; i++)
    {
        int clip = motion.cur.clip;
        int curFrame = motion.cur.curFrame;
       
        c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0)); //x 
        c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0)); //y  
        c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0)); //z 
        c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0)); //w 
        cur = matrix(c0, c1, c2, c3);
        
        n0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame+1, clip, 0)); //x 
        n1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame+1, clip, 0)); //y  
        n2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame+1, clip, 0)); //z 
        n3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame+1, clip, 0)); //w 
        next = matrix(n0, n1, n2, n3);
        
        curAnim = lerp(cur, next, motion.cur.time);
        
        clip = motion.next.clip;
        curFrame = motion.next.curFrame;
        
        if (clip > -1)  // 다음동작  ( Idle->Attack)
        {
            c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0)); //x 
            c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0)); //y  
            c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0)); //z 
            c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0)); //w 
            cur = matrix(c0, c1, c2, c3);
        
            n0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame + 1, clip, 0)); //x 
            n1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame + 1, clip, 0)); //y  
            n2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame + 1, clip, 0)); //z 
            n3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame + 1, clip, 0)); //w 
            next = matrix(n0, n1, n2, n3);
        
            nextAnim = lerp(cur, next, motion.next.time);
            curAnim = lerp(cur, next, motion.cur.time);
        }
        
        transform += mul(weights[i], curAnim);
    }
    
    return transform;
}