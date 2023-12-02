#pragma once

/////////////////////////////////////////////////////////////////
//                        World 담당      	    		       //
//                Object들 UpdateWorld 처리                     //
/////////////////////////////////////////////////////////////////

class Transform
{
public:
    Transform();
    ~Transform() = default;

public:
    void    UpdateWorld();   // W,V,P
    void    GUIRender();

public: // Getter
    Transform* GetParent();
    Matrix GetWorld()       { return m_world; }
    string GetTag()         { return m_tag; }

    // Vector
    Vector3 Right()         { return m_right.GetNormalized(); }
    Vector3 Left()          { return m_right.GetNormalized() * -1.0f; }
    Vector3 Up()            { return m_up.GetNormalized(); }
    Vector3 Down()          { return m_up.GetNormalized() * -1.0f; }
    Vector3 Forward()       { return m_forward.GetNormalized(); }
    Vector3 Back()          { return m_forward.GetNormalized() * -1.0f; }

    // 월드 Transform
    Vector3 GlobalPos()     { return m_globalPosition; }
    Vector3 GlobalScale()   { return m_globalScale; }
    Vector3 Pivot()         { return m_pivot; }

    // 로컬 Transform
    Vector3& Pos()      { return m_localPosition; }
    Vector3& Rot()      { return m_localRotation; }
    Vector3& Scale()    { return m_localScale; }

    bool IsActive()     { return m_isActive; }

public: // Setter
    void SetTag(string tag) { m_tag = tag; }
    void SetParent(Transform* transform) { m_parent = transform; }
    void SetPivot(Vector3 pivot) { m_pivot = pivot; }

    void SetActive(bool isActive) { m_isActive = isActive; }

    bool Active();

protected:
    string m_tag;

	// local Transform
    Vector3 m_localPosition   = { 0, 0, 0 };
    Vector3 m_localRotation   = { 0, 0, 0 };
    Vector3 m_localScale      = { 1, 1, 1 };

    bool    m_isActive = true;    

    Matrix  m_world = XMMatrixIdentity();

private:
    Transform*  m_parent = nullptr;
    Vector3     m_pivot;

    Vector3     m_right, m_up, m_forward;
    Vector3     m_globalPosition, m_globalScale;

    XMFLOAT4X4  m_matWorld = {}; // XMMATRIX 변환한 값
};