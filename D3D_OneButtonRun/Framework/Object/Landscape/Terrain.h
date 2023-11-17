#pragma once

class Terrain : public GameObject
{
private:
    typedef VertexUVNormalTangent VertexType;
    const float MAX_HEIGHT = 20.0f;

public:
    Terrain();
    Terrain(string mapFile);
    ~Terrain();

    void Render();

    float GetHeight(const Vector3& pos, Vector3* normal = nullptr);
    Vector3 Picking();

    Vector2 GetSize() { return Vector2(m_width, m_height); }
private:
    void MakeMesh();
    void MakeNormal();
    void MakeTangent();

private:
    UINT m_width, m_height;

    Mesh<VertexType>* m_mesh;

    Texture* m_heightMap;
    Texture* m_alphaMap;
    Texture* m_secondMap;
    Texture* m_thirdMap;
};