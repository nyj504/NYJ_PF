#include "Framework.h"
#include "InGameScene.h"
InGameScene::InGameScene() : isPaused(false)
{
	PlayerSingleton::Get();
	UIManager::Get();
	ItemManager::Get();
	MonsterManager::Get();

	EquipManager::Get();
	EquipManager::Get()->SetTarget(PLAYER->GetModelAnimator());
	
	CAM->SetTarget(PLAYER);
	CAM->TargetOptionLoad("FPSMode");
	CAM->SetFPSView(true);

	sky = new Sky();

	PLAYER->SetLocalPosition(0, 4, 0);
	
	curMusic = "BGM0";
}

InGameScene::~InGameScene()
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

void InGameScene::Update()
{
	isSceneActive = true;
	if (!Audio::Get()->IsPlaySound(curMusic))
	{
		silenceTimer += DELTA;
	}

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

	if (!Audio::Get()->IsPlaySound(curMusic) && isSceneActive && silenceTimer >= BGM_INTERVAL)
	{
		silenceTimer -= BGM_INTERVAL;
		ChangeBGM();
	}
}

void InGameScene::PreRender()
{
	UIManager::Get()->PreRender();
}

void InGameScene::Render()
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

void InGameScene::PostRender()
{
	BlockManager::Get()->PostRender();
	UIManager::Get()->PostRender();
}

void InGameScene::GUIRender()
{
	//UIManager::Get()->Edit();
	//
	//if (isPaused)
	//{
	//	/*ImGui::Begin("Pause Menu");
	//
	//	if (ImGui::Button("Save Game"))
	//	{
	//		MAP->Save(); 
	//	}
	//
	//	if (ImGui::Button("Resume"))
	//	{
	//		isPaused = false; 
	//	}
	//
	//	ImGui::End();*/
	//}
}

void InGameScene::ChangeBGM()
{
	musicIndex = (musicIndex + 1) % 3;
	string name = "BGM" + to_string(musicIndex);
	curMusic = name;
	Audio::Get()->Play(curMusic);
}
