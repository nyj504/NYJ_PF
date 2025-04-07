#include "Framework.h"
#include "LobbyScene.h"

LobbyScene::LobbyScene()
{
	ShowCursor(true);

	InitAudio();

	title = new Quad(L"Resources/Textures/Lobby/title.png");
	title->SetLocalPosition(Vector3(CENTER.x, CENTER.y + 100));
	title->UpdateWorld();

	background = new Quad(L"Resources/Textures/Lobby/panorama_0.png");
	background->SetLocalPosition(CENTER);
	background->SetLocalScale(Vector3(1.3f, 1.0f));
	background->UpdateWorld();

	Button* button = new Button(L"Resources/Textures/Lobby/lobbyButton.png");
	button->SetLocalPosition(Vector3(CENTER.x, CENTER.y));
	button->SetText("Singleplayer");
	button->SetEvent(bind(&LobbyScene::GameStart, this));
	button->UpdateWorld();

	buttons.push_back(button);

	Button* button1 = new Button(L"Resources/Textures/Lobby/lobbyButton.png");
	button1->SetLocalPosition(Vector3(CENTER.x, CENTER.y - 50));
	button1->SetText("Options...");
	//button1->SetEvent(bind(&PauseMenuUI::OnExitButtonClick, this));
	button1->UpdateWorld();

	buttons.push_back(button1);
	
	Audio::Get()->Play("Title");
}

LobbyScene::~LobbyScene()
{
	for (Button* button : buttons)
		delete button;
	
	delete title;
	delete background;
}

void LobbyScene::Update()
{
	for (Button* button : buttons)
		button->Update();

	if (KEY->Down('1'))
	{
		Audio::Get()->Play("Steve_knockback");
	}
	if (KEY->Down('2'))
	{
		Audio::Get()->Play("Steve_hurt");
	}
	if (KEY->Down('3'))
	{
		Audio::Get()->Play("Steve_hit");
	}
	if (KEY->Down('4'))
	{
		Audio::Get()->Play("step_cloth4");
	}
	if (KEY->Down('5'))
	{
		Audio::Get()->Play("step_stone1");
	}
	if (KEY->Down('6'))
	{
		Audio::Get()->Play("step_stone2");
	}
	if (KEY->Down('7'))
	{
		Audio::Get()->Play("step_cloth3");
	}
}

void LobbyScene::PreRender()
{
}

void LobbyScene::Render()
{
}

void LobbyScene::PostRender()
{
	background->Render();
	title->Render();

	for (Button* button : buttons)
		button->Render();
}

void LobbyScene::GUIRender()
{
	for (Button* button : buttons)
		button->Edit();
}

void LobbyScene::GameStart()
{
	ShowCursor(false);

	Audio::Get()->Stop("Title");
	
	SceneManager::Get()->Add("InGame");
	SceneManager::Get()->Remove("Lobby");
}

void LobbyScene::InitAudio()
{
	Audio::Get()->LoadAudioFiles("Resources/Sounds/BGM");
	Audio::Get()->LoadAudioFiles("Resources/Sounds/InGame/Block");
	Audio::Get()->LoadAudioFiles("Resources/Sounds/InGame/Chicken");
	Audio::Get()->LoadAudioFiles("Resources/Sounds/InGame/Dig");
	Audio::Get()->LoadAudioFiles("Resources/Sounds/InGame/Item");
	Audio::Get()->LoadAudioFiles("Resources/Sounds/InGame/Step");
	Audio::Get()->LoadAudioFiles("Resources/Sounds/InGame/Steve");
	Audio::Get()->LoadAudioFiles("Resources/Sounds/InGame/Zombie");
}
