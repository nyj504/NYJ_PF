#include "Framework.h"
#include "BlockTestScene.h"

BlockTestScene::BlockTestScene()
{
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
			for (int y = 0; y < 2; y++)
			{
				Vector3 pos = { (float)x, (float)y, (float)z };
				Block* block = new Block(2);
				block->SetLocalPosition(pos.x, pos.y, pos.z);
				block->UpdateWorld();
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
}

void BlockTestScene::PreRender()
{
}

void BlockTestScene::Render()
{
	instanceBuffer->Set(1);

	cube->RenderInstanced(instanceData.size());
	//cube->Render();
}

void BlockTestScene::PostRender()
{
}

void BlockTestScene::GUIRender()
{
}
