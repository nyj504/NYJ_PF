#include "Framework.h"
#include "TestScene.h"

TestScene::TestScene() : isPaused(false)
{
	Init();
}

TestScene::~TestScene()
{
	PlayerSingleton::Delete();
	InventorySingleton::Delete();
	BlockManager::Delete();
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
		PLAYER->Update();
		BlockManager::Get()->Update();
		UIManager::Get()->Update();
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
	}
}

void TestScene::PostRender()
{
	BlockManager::Get()->PostRender();
	UIManager::Get()->PostRender();

	string position = "Pos X:" + to_string((int)PLAYER->GetGlobalPosition().x) +
		" Pos Y:" + to_string((int)PLAYER->GetGlobalPosition().y) + " Pos Z:" + to_string((int)PLAYER->GetGlobalPosition().z);
	Font::Get()->RenderText(position, { 150, SCREEN_HEIGHT - 30 });
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

void TestScene::Init()
{
	PlayerSingleton::Get();
	InventorySingleton::Get();
	UIManager::Get();
	BlockManager::Get();

	PLAYER->SetLocalPosition(0, 1, 0);
	CAM->SetTarget(PLAYER);
	CAM->TargetOptionLoad("FPSMode");
	CAM->SetFPSView(true);

	skybox = new Skybox(L"Resources/Textures/Landscape/SpaceSky.dds");
}
