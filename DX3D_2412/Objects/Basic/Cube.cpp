#include "Framework.h"

Cube::Cube(Vector3 size) : size(size)
{
    MakeMesh();
    MakeNormal();
    MakeTangent();
    mesh->CreateMesh();    

    collider = new BoxCollider(size);
    collider->SetParent(this);
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


void Cube::ApplyCharacterUVMapping(Vector2 startUV, Vector3 size, int atlasSize)
{
    vector<VertexType>& vertices = mesh->GetVertices();

    float startU = (float)startUV.x / atlasSize;
    float startV = (float)startUV.y/ atlasSize;

    float width = (float)size.x / atlasSize;
    float height = (float)size.y / atlasSize;
    float depth = (float)size.z / atlasSize;

    for (int i = 0; i < 6; i++)
    {
        int startIdx = i * 4;
        
        float tempU = startU;
        float tempV = startV;

        if (i == 0) // front startUV위치
        {
            vertices[startIdx + 0].uv = { tempU, tempV };
            vertices[startIdx + 1].uv = { tempU + width, tempV };
            vertices[startIdx + 2].uv = { tempU, tempV + height };
            vertices[startIdx + 3].uv = { tempU + width, tempV + height };
        }
        if (i == 1) // right
        {
            tempU += width;
            vertices[startIdx + 0].uv = { tempU, tempV };
            vertices[startIdx + 1].uv = { tempU + depth, tempV };
            vertices[startIdx + 2].uv = { tempU, tempV + height };
            vertices[startIdx + 3].uv = { tempU + depth, tempV + height };
        }
        if (i == 2) // up
        {
            tempV -= depth;
            vertices[startIdx + 0].uv = { tempU, tempV };
            vertices[startIdx + 1].uv = { tempU + width, tempV };
            vertices[startIdx + 2].uv = { tempU, tempV + depth };
            vertices[startIdx + 3].uv = { tempU + width, tempV + depth };
        }
        if (i == 3) // back
        {
            tempU += width + depth;
            vertices[startIdx + 0].uv = { tempU, tempV };
            vertices[startIdx + 1].uv = { tempU + width, tempV };
            vertices[startIdx + 2].uv = { tempU, tempV + height };
            vertices[startIdx + 3].uv = { tempU + width, tempV + height };
        }
        if (i == 4) // left
        {
            tempU -= depth;
            vertices[startIdx + 0].uv = { tempU, tempV };
            vertices[startIdx + 1].uv = { tempU + depth, tempV };
            vertices[startIdx + 2].uv = { tempU, tempV + height };
            vertices[startIdx + 3].uv = { tempU + depth, tempV + height };
        }
        if (i == 5) // down
        {
            tempU += width;
            tempV -= depth;
            vertices[startIdx + 0].uv = { tempU, tempV };
            vertices[startIdx + 1].uv = { tempU + width, tempV };
            vertices[startIdx + 2].uv = { tempU, tempV + depth };
            vertices[startIdx + 3].uv = { tempU + width, tempV + depth };
        }
    }
    mesh->UpdateVertices();
}

void Cube::MakeMesh()
{
    //Polygon : 3차원에서의 정점 3개로 이루어진 삼각형
    //-> 시계방향을 앞면으로 해서 앞면만 출력함.

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

//void Cube::Update()
//{
//    if (GetAsyncKeyState('D'))
//        pos.x += 0.001f;
//    if (GetAsyncKeyState('A'))
//        pos.x -= 0.001f;
//
//    Matrix T = XMMatrixTranslation(pos.x, pos.y, pos.z);
//
//
//    if (GetAsyncKeyState('T'))
//        scale.y += 0.001f;
//    if (GetAsyncKeyState('G'))
//        scale.y -= 0.001f;
//
//    Matrix S = XMMatrixScaling(scale.x, scale.y, scale.z);
//
//    static float angle = 0.0f;
//
//    if (GetAsyncKeyState(VK_UP))
//        rot.z += 0.001f;
//    if (GetAsyncKeyState(VK_DOWN))
//        rot.z -= 0.001f;
//
//    worldMat._11 = cos(angle);
//    worldMat._13 = sin(angle);
//    worldMat._31 = -sin(angle);
//    worldMat._33 = cos(angle);
//
//    Matrix R = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
//
//    //world = XMLoadFloat4x4(&worldMat);
//    world = S * R * T;
//}