#include "Framework.h"
#include "BlockTestScene.h"

BlockTestScene::BlockTestScene()
{
	PlayerSingleton::Get();
	PLAYER->SetLocalPosition(4, 4, 4);
	CAM->SetTarget(PLAYER);
	CAM->TargetOptionLoad("FPSMode");
	CAM->SetFPSView(true);

	ShowCursor(true);

	cube = new Cube();

	cube->GetMaterial()->SetShader(L"Instancing/Instancing.hlsl");
	//cube->ApplyObjectUVMapping();
	cube->GetMaterial()->SetDiffuseMap(L"Resources/Textures/BlockTexture/Atlasmap.png");
	cube->UpdateWorld();

	for (int x = 0; x < 10; x++)
	{
		for (int z = 0; z < 10; z++)
		{
			int y = (x + z) / 2; 

			Vector3 pos = { (float)x, (float)y, (float)z };
			Block* block = new Block(2);
			block->SetLocalPosition(pos);
			block->UpdateWorld();
			block->EnableCollider();
			blocks.push_back(block);

			InstanceData visibleInstanceData;
			UVInfo uvInfo = block->GetUVInfo();

			visibleInstanceData.transform = XMMatrixTranslation(pos.x, pos.y, pos.z);
			visibleInstanceData.transform = XMMatrixTranspose(visibleInstanceData.transform);

			visibleInstanceData.curFrame = uvInfo.uvStart;
			visibleInstanceData.maxFrame = uvInfo.uvEnd;

			instanceData.push_back(visibleInstanceData);
			
		}
	}

	instanceBuffer = new VertexBuffer(instanceData.data(), sizeof(InstanceData), (UINT)instanceData.size());
}

BlockTestScene::~BlockTestScene()
{
	delete cube;
	delete instanceBuffer;
}

void BlockTestScene::Update()
{
	for (Block* block : blocks)
	{
		block->Update();
	}

	PLAYER->Update();
}	

void BlockTestScene::PreRender()
{
}

void BlockTestScene::Render()
{
	instanceBuffer->Set(1);

	cube->RenderInstanced(instanceData.size());

	for (Block* block : blocks)
	{
		block->Render();
	}

	PLAYER->Render();
}

void BlockTestScene::PostRender()
{
}

void BlockTestScene::GUIRender()
{
}
