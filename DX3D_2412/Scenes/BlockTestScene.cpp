#include "Framework.h"
#include "BlockTestScene.h"

BlockTestScene::BlockTestScene()
{
	PlayerSingleton::Get();
	UIManager::Get();
	InventorySingleton::Get();
	EquipManager::Get();
	EquipManager::Get()->SetTarget(PLAYER->GetModelAnimator());
	
	hpBar = new HUDBar();
	hpBar->Create("Resources/Textures/UI/heart.png", Vector3(CENTER.x - 260 , CENTER.y - 280));
	
	armorBar = new HUDBar();
	armorBar->Create("Resources/Textures/UI/armor_empty.png", Vector3(CENTER.x - 260, CENTER.y - 256));
	//armorBar->UpdateBar(1);
	
	hungerBar = new HUDBar();
	hungerBar->Create("Resources/Textures/UI/hunger_full.png", Vector3(CENTER.x + 86, CENTER.y - 280));

	PLAYER->SetLocalPosition(2, 4, 2);

	CAM->SetTarget(PLAYER);
	CAM->TargetOptionLoad("FPSMode");
	CAM->SetFPSView(true);

	skybox = new Skybox(L"Resources/Textures/Skybox/Night_4096x2048.dds");

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
			Vector3 pos = { (float)x, 0, (float)z };
			Block* block = new Block(16);
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
	//delete item;
	
}

void BlockTestScene::Update()
{
	if (KEY->Down(VK_F2))
	{
		CAM->SetTarget(nullptr);
	}

	if (KEY->Down(VK_F3))
	{
		CAM->SetTarget(PLAYER);
		CAM->TargetOptionLoad("FPSMode");
		CAM->SetFPSView(true);
	}

	if (KEY->Down(VK_F4))
	{
		CAM->SetTarget(PLAYER);
		CAM->TargetOptionLoad("QuaterViewMode");
		CAM->SetQuaterView(true);
	}

	for (Block* block : blocks)
	{
		block->Update();
	}

	PLAYER->Update();
	UIManager::Get()->Update();
	EquipManager::Get()->Update();
	
}	

void BlockTestScene::PreRender()
{
}

void BlockTestScene::Render()
{
	skybox->Render();

	instanceBuffer->Set(1);

	Environment::Get()->SetAlphaBlend(true);
	cube->RenderInstanced(instanceData.size());
	Environment::Get()->SetAlphaBlend(false);

	for (Block* block : blocks)
	{
		block->Render();
	}

	PLAYER->Render();
	EquipManager::Get()->Render();
	ItemManager::Get()->Render();

}

void BlockTestScene::PostRender()
{
	UIManager::Get()->PostRender();
	hpBar->Render();
	armorBar->Render();
	hungerBar->Render();
}

void BlockTestScene::GUIRender()
{
}
