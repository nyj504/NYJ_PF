#include "Framework.h"

#include "Scenes/GridScene.h"
#include "Scenes/TestScene.h"
#include "Scenes/LightScene.h"
#include "Scenes/TerrainScene.h"
#include "Scenes/LobbyScene.h"
#include "Scenes/InstancingScene.h"
#include "Scenes/ModelRenderScene.h"
#include "Scenes/ModelExportScene.h"
#include "Scenes/ModelInstancingScene.h"
#include "Scenes/ModelAnimationScene.h"
#include "Scenes/BlockTestScene.h"
#include "Scenes/ModelEquipScene.h"
#include "Scenes/WeatherScene.h"
#include "Scenes/ParticleEditorScene.h"

GameManager::GameManager()
{
	//SCENE->Create("Grid", new GridScene());
	//SCENE->Create("Start", new LightScene());
	//SCENE->Create("Start", new InstancingScene());
	//SCENE->Create("Start", new TerrainScene());

	//SCENE->Create("Start", new ModelAnimationScene());
	//SCENE->Create("Start", new ModelInstancingScene());
	//SCENE->Create("Start", new BlockTestScene());
	//SCENE->Create("Start", new ModelEquipScene());
	 
	//SCENE->Create("Start", new ParticleEditorScene());
	//SCENE->Create("Start", new WeatherScene());

	//SCENE->Add("Start");
	//SCENE->Add("Grid");
	
	//SCENE->Create("Model", new ModelExportScene());
	//SCENE->Create("ModelRender", new ModelRenderScene());
	
	//SCENE->Add("Model");
	//SCENE->Add("ModelRender");
	
	SCENE->Create("Lobby", new LobbyScene());
	SCENE->Create("Test", new TestScene());
	
	SCENE->Add("Lobby");
	
	Create();
}

GameManager::~GameManager()
{
	Release();
}

void GameManager::Update()
{
	Keyboard::Get()->Update();
	Timer::Get()->Update();

	SCENE->Update();

	Environment::Get()->Update();
}

void GameManager::Render()
{
	SCENE->PreRender();

	Device::Get()->Clear();

	Environment::Get()->SetRender();
	SCENE->Render();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Font::Get()->GetDC()->BeginDraw();

	string fps = "FPS : " + to_string(Timer::Get()->GetFPS());
	Font::Get()->RenderText(fps, { 100, SCREEN_HEIGHT - 10 });

	//string position = "Pos X:" + to_string((int)PLAYER->GetGlobalPosition().x) +
	//	" Pos Y:" + to_string((int)PLAYER->GetGlobalPosition().y) + " Pos Z:" + to_string((int)PLAYER->GetGlobalPosition().z);
	//Font::Get()->RenderText(position, { 150, SCREEN_HEIGHT - 30 });

	Environment::Get()->SetPostRender();
	SCENE->PostRender();

	Environment::Get()->Edit();
	SCENE->GUIRender();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	Font::Get()->GetDC()->EndDraw();

	Device::Get()->Present();
}

void GameManager::Create()
{
	srand((UINT)(time(0)));

	Device::Get();
	Environment::Get();
	Timer::Get();
	Keyboard::Get();
	SceneManager::Get();
	EventManager::Get();
	Font::Get();
	Observer::Get();

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(DEVICE, DC);

	Font::Get()->AddColor("White", 1, 1, 1);
	Font::Get()->AddStyle("Default", L"µ¸¿òÃ¼");

	Font::Get()->SetColor("White");
	Font::Get()->SetStyle("Default");
}

void GameManager::Release()
{
	Device::Delete();	
	Environment::Delete();
	Timer::Delete();
	EventManager::Delete();
	Keyboard::Delete();
	SceneManager::Delete();
	Font::Delete();
	PlayerSingleton::Delete();
	InventorySingleton::Delete();
	Observer::Delete();

	Shader::Delete();
	Texture::Delete();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
}

