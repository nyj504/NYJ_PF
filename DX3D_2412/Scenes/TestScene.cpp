#include "Framework.h"
#include "TestScene.h"
TestScene::TestScene() : isPaused(false)
{
	PlayerSingleton::Get();
	UIManager::Get();
	InventorySingleton::Get();
	ItemManager::Get();

	EquipManager::Get();
	EquipManager::Get()->SetTarget(PLAYER->GetModelAnimator());
	
	zombie = new Monster();
	zombie->SetLocalPosition(3, 3, 3);

	animal = new Animal();
	animal->SetLocalPosition(5, 3, 5);

	CAM->SetTarget(PLAYER);
	CAM->TargetOptionLoad("FPSMode");
	CAM->SetFPSView(true);

	skybox = new Skybox(L"Resources/Textures/Skybox/BlueSunset_4096x2048.dds");

	PLAYER->SetLocalPosition(0, 4, 0);
}

TestScene::~TestScene()
{
	BlockManager::Delete();
	EquipManager::Delete();
	UIManager::Delete();
	EquipManager::Delete();

	delete zombie;
	delete animal;
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

	if (KEY->Down(VK_F4))
	{
		CAM->SetTarget(PLAYER);
		CAM->TargetOptionLoad("QuaterViewMode");
		CAM->SetQuaterView(true);
	}

	if (!isPaused)
	{
		BlockManager::Get()->Update();
		PLAYER->Update();
		UIManager::Get()->Update();
		EquipManager::Get()->Update();
		ItemManager::Get()->Update();
		zombie->Update();
		animal->Update();
;	}
}

void TestScene::PreRender()
{
}

void TestScene::Render()
{
	skybox->Render();

	if (!isPaused)
	{
		BlockManager::Get()->Render();
		PLAYER->Render();
		EquipManager::Get()->Render();
		ItemManager::Get()->Render();
		zombie->Render();
		animal->Render();
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
