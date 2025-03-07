#include "Framework.h"
#include "LobbyScene.h"

LobbyScene::LobbyScene()
{
	ShowCursor(true);

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


	//Audio::Get()->Play("Title");
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
	SceneManager::Get()->Add("Test");
	SceneManager::Get()->Remove("Lobby");
}