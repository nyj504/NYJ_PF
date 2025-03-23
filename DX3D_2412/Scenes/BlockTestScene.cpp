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
	CAM->TargetOptionLoad("QuaterViewMode");
	CAM->SetFPSView(true);

	item = new Quad(L"Resources/Textures/Item/stick.png", Vector2(0.1f, 0.1f));
	item->SetLocalPosition(Vector3(3, 4, 3));
	item->UpdateWorld();

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
	delete item;
	
}

void BlockTestScene::Update()
{
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
	instanceBuffer->Set(1);

	cube->RenderInstanced(instanceData.size());

	for (Block* block : blocks)
	{
		block->Render();
	}

	PLAYER->Render();
	EquipManager::Get()->Render();
	item->Render();
	

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
