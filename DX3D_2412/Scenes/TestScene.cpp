#include "Framework.h"
#include "TestScene.h"

TestScene::TestScene() : isPaused(false)
{
	UIManager::Get();

	zombie = new Character();
	zombie->SetLocalPosition(9, 1, 8);
	zombie->UpdateWorld();

	CAM->SetTarget(PLAYER);
	CAM->TargetOptionLoad("FPSMode");
	CAM->SetFPSView(true);

	skybox = new Skybox(L"Resources/Textures/Landscape/SpaceSky.dds");

	PLAYER->SetLocalPosition(0, 1, 0);
}

TestScene::~TestScene()
{
	BlockManager::Delete();
	UIManager::Delete();

	delete zombie;
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
		PLAYER->Update();
		BlockManager::Get()->Update();
		UIManager::Get()->Update();

		zombie->Update();
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

		zombie->Render();
	}
}

void TestScene::PostRender()
{
	BlockManager::Get()->PostRender();
	UIManager::Get()->PostRender();

	PLAYER->PostRender();
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
