#include "Framework.h"
#include "TestScene.h"
TestScene::TestScene() : isPaused(false)
{
	PlayerSingleton::Get();
	UIManager::Get();
	InventorySingleton::Get();
	ItemManager::Get();
	MonsterManager::Get();

	EquipManager::Get();
	EquipManager::Get()->SetTarget(PLAYER->GetModelAnimator());
	
	CAM->SetTarget(PLAYER);
	CAM->TargetOptionLoad("FPSMode");
	CAM->SetFPSView(true);

	sky = new Sky();

	PLAYER->SetLocalPosition(0, 4, 0);

	MonsterManager::Get()->Spawn();
}

TestScene::~TestScene()
{
	BlockManager::Delete();
	EquipManager::Delete();
	UIManager::Delete();
	EquipManager::Delete();
	MonsterManager::Delete();
	PlayerSingleton::Delete();
	InventorySingleton::Delete();
	delete sky;
}

void TestScene::Update()
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

	if (KEY->Down(VK_F10))
	{
		CAM->SetTarget(PLAYER);
		CAM->TargetOptionLoad("RenderTargetMode");
		CAM->SetQuaterView(true);
	}

	if (!isPaused)
	{
		BlockManager::Get()->Update();
		PLAYER->Update();
		UIManager::Get()->Update();
		EquipManager::Get()->Update();
		ItemManager::Get()->Update();
		MonsterManager::Get()->Update();
		sky->Update();
;	}
}

void TestScene::PreRender()
{
	UIManager::Get()->PreRender();
}

void TestScene::Render()
{
	sky->Render();

	if (!isPaused)
	{
		BlockManager::Get()->Render();
		PLAYER->Render();
		EquipManager::Get()->Render();
		ItemManager::Get()->Render();
		MonsterManager::Get()->Render();
	}
}

void TestScene::PostRender()
{
	BlockManager::Get()->PostRender();
	UIManager::Get()->PostRender();
}

void TestScene::GUIRender()
{
	UIManager::Get()->Edit();

	if (isPaused)
	{
		/*ImGui::Begin("Pause Menu");

		if (ImGui::Button("Save Game"))
		{
			MAP->Save(); 
		}

		if (ImGui::Button("Resume"))
		{
			isPaused = false; 
		}

		ImGui::End();*/
	}
}
