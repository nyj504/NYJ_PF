#include "Framework.h"

Quad::Quad(Vector2 size)
{
    tag = "Quad";

    material->SetDiffuseMap(L"Resources/Textures/Colors/White.png");

    vector<VertexUV>& vertices = mesh->GetVertices();
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, 0.0f, 0, 0);//0
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, 0.0f, 1, 0);//1
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, 0.0f, 0, 1);//2
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, 0.0f, 1, 1);//3

    vector<UINT>& indices = mesh->GetIndices();
    indices = {
        0, 1, 2, 2, 1, 3,
    };

    mesh->CreateMesh();
}

Quad::Quad(wstring texture)
{
    tag = "Quad";

    material->SetShader(L"Basic/Texture.hlsl");
    material->SetDiffuseMap(texture);
    this->size = material->GetDiffuseMap()->GetSize();

    vector<VertexUV>& vertices = mesh->GetVertices();
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, 0.0f, 0, 0);//0
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, 0.0f, 1, 0);//1
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, 0.0f, 0, 1);//2
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, 0.0f, 1, 1);//3    

    vector<UINT>& indices = mesh->GetIndices();
    indices = {
        0, 1, 2, 2, 1, 3,
    };

    mesh->CreateMesh();
}

Quad::Quad(string name, Vector2 size)
{
    tag = name;

    string path = "Resources/Textures/Itemx2/" + name + ".png";

    material->SetShader(L"Basic/Texture.hlsl");
    material->SetDiffuseMap(Utility::ToWString(path));

    vector<VertexUV>& vertices = mesh->GetVertices();
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, 0.0f, 0, 0);//0
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, 0.0f, 1, 0);//1
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, 0.0f, 0, 1);//2
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, 0.0f, 1, 1);//3    

    vector<UINT>& indices = mesh->GetIndices();
    indices = {
        0, 1, 2, 2, 1, 3,
    };

    mesh->CreateMesh();
}

