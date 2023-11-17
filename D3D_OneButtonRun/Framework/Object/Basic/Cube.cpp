#include   "framework.h"
#include   "Cube.h"

Cube::Cube(wstring strShader,Vector3 size) 
    : GameObject(strShader)
{
	m_tag = "Cube";
	m_mesh = new Mesh<VertexType>();
	CreateVertex(size);
}

Cube::~Cube()
{
    SAFE_DELETE(m_mesh);
}

void Cube::Render()
{
    if (!m_isActive) return;
    SetRender();
    m_mesh->Draw();
}

void Cube::CreateVertex(Vector3 size)
{
	vector<VertexType>& vertices = m_mesh->GetVertices();

    //Front
    vertices.emplace_back(-size.x, -size.y, -size.z, 0, 1);
    vertices.emplace_back(-size.x, +size.y, -size.z, 0, 0);
    vertices.emplace_back(+size.x, -size.y, -size.z, 1, 1);
    vertices.emplace_back(+size.x, +size.y, -size.z, 1, 0);

    //Up
    vertices.emplace_back(-size.x, +size.y, -size.z, 0, 1);
    vertices.emplace_back(-size.x, +size.y, +size.z, 0, 0);
    vertices.emplace_back(+size.x, +size.y, -size.z, 1, 1);
    vertices.emplace_back(+size.x, +size.y, +size.z, 1, 0);

    //Left
    vertices.emplace_back(+size.x, +size.y, -size.z, 0, 1);
    vertices.emplace_back(+size.x, +size.y, +size.z, 0, 0);
    vertices.emplace_back(+size.x, -size.y, -size.z, 1, 1);
    vertices.emplace_back(+size.x, -size.y, +size.z, 1, 0);

    //Right
    vertices.emplace_back(-size.x, +size.y, -size.z, 0, 1);
    vertices.emplace_back(-size.x, -size.y, -size.z, 0, 0);
    vertices.emplace_back(-size.x, +size.y, +size.z, 1, 1);
    vertices.emplace_back(-size.x, -size.y, +size.z, 1, 0);

    //Back
    vertices.emplace_back(-size.x, +size.y, +size.z, 0, 1);
    vertices.emplace_back(-size.x, -size.y, +size.z, 0, 0);
    vertices.emplace_back(+size.x, +size.y, +size.z, 1, 1);
    vertices.emplace_back(+size.x, -size.y, +size.z, 1, 0);

    //Down
    vertices.emplace_back(-size.x, -size.y, -size.z, 0, 1);
    vertices.emplace_back(+size.x, -size.y, -size.z, 0, 0);
    vertices.emplace_back(-size.x, -size.y, +size.z, 1, 1);
    vertices.emplace_back(+size.x, -size.y, +size.z, 1, 0);

    vector<UINT>& indices = m_mesh->GetIndices();
    indices = {
        //Front
        0, 1, 2, 2, 1, 3,
        //Up
        4, 5, 6, 6, 5, 7,
        //Right
        8, 9, 10, 10, 9, 11,
        //Left
        12, 13, 14, 14, 13, 15,
        //Back
        16, 17, 18, 18, 17, 19,
        //Down
        20, 21, 22, 22, 21, 23
    };

    m_mesh->CreateMesh();
}
