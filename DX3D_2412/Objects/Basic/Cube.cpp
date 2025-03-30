#include "Framework.h"

Cube::Cube(Vector3 size) : size(size)
{
    MakeMesh();
    MakeNormal();
    MakeTangent();
    mesh->CreateMesh();    

    collider = new BoxCollider(size);
    collider->SetParent(this);
    collider->UpdateWorld();
}

void Cube::Update()
{
    if (!IsActive()) return;

    UpdateWorld();
    collider->UpdateWorld();
}

void Cube::Render()
{
     if (!IsActive()) return;

    GameObject::Render();
    collider->Render();
}

void Cube::RenderInstanced(UINT count)
{
    GameObject::RenderInstanced(count);

    //collider->GetMaterial()->SetShader(L"Instancing/ColliderInstancing.hlsl");
    //collider->RenderInstanced(count, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void Cube::ApplyObjectUVMapping()
{
    float atlasUSize = 0.25f;
    float atlasVSize = 1.0f / 7.0f;

    vector<VertexType>& vertices = mesh->GetVertices();

    for (int i = 0; i < 6; i++)
    {
        int startIdx = i * 4;

        if (i == 0) // front startUV위치
        {
            vertices[startIdx + 0].uv = { 0, 0 };
            vertices[startIdx + 1].uv = { atlasUSize, 0 };
            vertices[startIdx + 2].uv = { 0, atlasVSize };
            vertices[startIdx + 3].uv = { atlasUSize, atlasVSize };
        }
        else if (i == 2) // up
        {
            vertices[startIdx + 0].uv = { 2 * atlasUSize, 0 };
            vertices[startIdx + 1].uv = { 3 * atlasUSize, 0 };
            vertices[startIdx + 2].uv = { 2 * atlasUSize, atlasVSize };
            vertices[startIdx + 3].uv = { 3 * atlasUSize, atlasVSize };
        }

        else if (i == 5) // down
        {
            vertices[startIdx + 0].uv = { 3 * atlasUSize, 0 };
            vertices[startIdx + 1].uv = { 1, 0 };
            vertices[startIdx + 2].uv = { 3 * atlasUSize, atlasVSize };
            vertices[startIdx + 3].uv = { 1, atlasVSize };
        }
        else
        {
            vertices[startIdx + 0].uv = { atlasUSize, 0 };
            vertices[startIdx + 1].uv = { 2 * atlasUSize, 0 };
            vertices[startIdx + 2].uv = { atlasUSize, atlasVSize };
            vertices[startIdx + 3].uv = { 2 * atlasUSize, atlasVSize };
        }
    }
    mesh->UpdateVertices();
}


void Cube::ApplyBlockUVMapping()
{
    float atlasUSize = 0.25f;

    vector<VertexType>& vertices = mesh->GetVertices();

    for (int i = 0; i < 6; i++)
    {
        int startIdx = i * 4;

        if (i == 0) // front startUV위치
        {
            vertices[startIdx + 0].uv = { 0, 0 };
            vertices[startIdx + 1].uv = { atlasUSize, 0 };
            vertices[startIdx + 2].uv = { 0, 1 };
            vertices[startIdx + 3].uv = { atlasUSize, 1 };
        }
        else if (i == 2) // up
        {
            vertices[startIdx + 0].uv = { 2 * atlasUSize, 0 };
            vertices[startIdx + 1].uv = { 3 * atlasUSize, 0 };
            vertices[startIdx + 2].uv = { 2 * atlasUSize, 1 };
            vertices[startIdx + 3].uv = { 3 * atlasUSize, 1 };
        }

        else if (i == 5) // down
        {
            vertices[startIdx + 0].uv = { 3 * atlasUSize, 0 };
            vertices[startIdx + 1].uv = { 1, 0 };
            vertices[startIdx + 2].uv = { 3 * atlasUSize, 1 };
            vertices[startIdx + 3].uv = { 1, 1 };
        }
        else
        {
            vertices[startIdx + 0].uv = { atlasUSize, 0 };
            vertices[startIdx + 1].uv = { 2 * atlasUSize, 0 };
            vertices[startIdx + 2].uv = { atlasUSize, 1 };
            vertices[startIdx + 3].uv = { 2 * atlasUSize, 1 };
        }
    }
    mesh->UpdateVertices();
}

void Cube::MakeMesh()
{
    vector<VertexType>& vertices = mesh->GetVertices();

    //Front
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 0, 0);//0
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 1, 0);//1
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 0, 1);//2
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 1, 1);//3

    //Right
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 0, 0);//1
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 1, 0);//5
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 0, 1);//3
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 1, 1);//7

    //Up
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 0, 0);//4
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 1, 0);//5
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 0, 1);//0
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 1, 1);//1

    //Back
    vertices.emplace_back(+0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 1, 0);
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 0, 0);//4    
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 1, 1);//7
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 0, 1);//6


    //Left
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, +0.5f * size.z, 1, 0);//4 18
    vertices.emplace_back(-0.5f * size.x, +0.5f * size.y, -0.5f * size.z, 0, 0);//0 16
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 1, 1);//6 19
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 0, 1);//2 17


    //Down
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 0, 0);//6    
    vertices.emplace_back(-0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 0, 1);//2
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, +0.5f * size.z, 1, 0);//7
    vertices.emplace_back(+0.5f * size.x, -0.5f * size.y, -0.5f * size.z, 1, 1);//3

    vector<UINT>& indices = mesh->GetIndices();
    indices = {
        //Front
        0, 1, 2, 2, 1, 3,
        //Right
        4, 5, 6, 6, 5, 7,
        //Up
        8, 9, 10, 10, 9, 11,
        //Back
        12, 13, 14, 14, 13, 15,
        //Left
        16, 17, 18, 18, 17, 19,
        //Down
        20, 21, 22, 22, 21, 23
    };
}