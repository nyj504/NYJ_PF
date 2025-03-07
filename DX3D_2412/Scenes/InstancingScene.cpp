#include "Framework.h"
#include "InstancingScene.h"

InstancingScene::InstancingScene()
{
    /*
    quads.resize(COUNT);
    for (Quad*& quad : quads)
    {
        quad = new Quad();
        quad->GetMaterial()->SetDiffuseMap(L"Textures/test.jpg");

        Vector3 pos(Random(-10.0f, 10.0f), Random(-10.0f, 10.0f));
        quad->SetLocalPosition(pos);

        quad->UpdateWorld();
    }
    */

    quad = new Quad(L"Resources/Textures/test.jpg");
    quad->GetMaterial()->SetShader(L"Instancing/Instancing.hlsl");

    instanceData.resize(COUNT);

    for (Data& data : instanceData)
    {
        float x = GameMath::Random(-10.0f, 10.0f);
        float y = GameMath::Random(-10.0f, 10.0f);

        data.transform = XMMatrixTranslation(x, y, 0.0f);
        data.transform = XMMatrixTranspose(data.transform);

        data.color.x = GameMath::Random(0.0f, 1.0f);
        data.color.y = GameMath::Random(0.0f, 1.0f);
        data.color.z = GameMath::Random(0.0f, 1.0f);
        data.color.w = 1;
    }

    instanceBuffer = new VertexBuffer(instanceData.data(), sizeof(Data), COUNT);
}

InstancingScene::~InstancingScene()
{
    //for (Quad* quad : quads)
    //    delete quad;

    delete quad;
    delete instanceBuffer;
}

void InstancingScene::Update()
{
}

void InstancingScene::PreRender()
{
}

void InstancingScene::Render()
{
    //for (Quad* quad : quads)
    //    quad->Render();

    instanceBuffer->Set(1);

    quad->RenderInstanced(COUNT);
}

void InstancingScene::PostRender()
{
}

void InstancingScene::GUIRender()
{
}
