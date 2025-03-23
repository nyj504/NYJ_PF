#include "Framework.h"
#include "TestScene.h"

TestScene::TestScene() : isPaused(false)
{
	PlayerSingleton::Get();
	UIManager::Get();
	InventorySingleton::Get();

	EquipManager::Get();
	EquipManager::Get()->SetTarget(PLAYER->GetModelAnimator());
	
	CAM->SetTarget(PLAYER);
	CAM->TargetOptionLoad("FPSMode");
	CAM->SetFPSView(true);

	skybox = new Skybox(L"Resources/Textures/Landscape/SpaceSky.dds");

	PLAYER->SetLocalPosition(0, 3, 0);
}

TestScene::~TestScene()
{
	BlockManager::Delete();
	EquipManager::Delete();
	UIManager::Delete();
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
	}
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
