#include "Framework.h"

#include "Scenes/GridScene.h"
#include "Scenes/InGameScene.h"
#include "Scenes/LobbyScene.h"
#include "Scenes/ParticleEditorScene.h"
#include "Scenes/ModelAnimatorScene.h"

GameManager::GameManager()
{
	//SCENE->Create("Grid", new GridScene());
	////SCENE->Create("Start", new ParticleEditorScene());
	//SCENE->Create("Start", new ModelAnimationScene());
	////
	//SCENE->Add("Start");
	//SCENE->Add("Grid");
	
	SCENE->Create("Lobby", new LobbyScene());
	SCENE->Create("InGame", new InGameScene());
	
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
	Audio::Get()->Update();

	SCENE->Update();

	Environment::Get()->Update();
}

void GameManager::Render()
{
	SCENE->PreRender();

	Device::Get()->Clear();

	Environment::Get()->SetRender();
	Environment::Get()->SetViewport();
	SCENE->Render();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Font::Get()->GetDC()->BeginDraw();

	Environment::Get()->SetPostRender();
	SCENE->PostRender();

	//Environment::Get()->Edit();
	//SCENE->GUIRender();

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
	Audio::Get();

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
	Observer::Delete();
	Audio::Delete();

	Shader::Delete();
	Texture::Delete();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
}

