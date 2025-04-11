#include "Framework.h"
#include "LobbyScene.h"

LobbyScene::LobbyScene()
{
	timeBuffer = new TimeBuffer();

	BlockManager::Get();
	InventorySingleton::Get();

	ShowCursor(true);

	InitAudio();

	title = new Quad(L"Resources/Textures/Lobby/title.png");
	title->SetLocalPosition(Vector3(CENTER.x, CENTER.y + 100));
	title->UpdateWorld();

	background = new Quad(L"Resources/Textures/Lobby/panorama_0.png");
	background->GetMaterial()->SetShader(L"UI/Lobby.hlsl");
	background->SetLocalPosition(CENTER);
	background->SetLocalScale(Vector3(1.3f, 1.0f));
	background->UpdateWorld();

	for (int i = 0; i < PANORAMA_SIZE; i++)
	{
		string path = "Resources/Textures/Lobby/panorama_" + to_string(i) + ".png";
		backgrounds[i] = path;
	}

	lobbyStart = Texture::Add(L"Resources/Textures/Lobby/panorama_0.png");
	lobbyEnd = Texture::Add(L"Resources/Textures/Lobby/panorama_1.png");

	Button* button = new Button(L"Resources/Textures/Lobby/lobbyButton.png");
	button->SetLocalPosition(Vector3(CENTER.x, CENTER.y));
	button->SetText("Singleplayer");
	button->SetEvent(bind(&LobbyScene::GameStart, this));
	button->UpdateWorld();

	buttons.push_back(button);

	Button* button1 = new Button(L"Resources/Textures/Lobby/lobbyButton.png");
	button1->SetLocalPosition(Vector3(CENTER.x, CENTER.y - 50));
	button1->SetText("Load Saved Game");
	button1->SetEvent(bind(&LobbyScene::LoadGame, this));
	button1->UpdateWorld();

	buttons.push_back(button1);
	
	Audio::Get()->Play("Title");
}

LobbyScene::~LobbyScene()
{
	for (Button* button : buttons)
		delete button;

	delete title;
	delete timeBuffer;
}

void LobbyScene::Update()
{
	lerpTime += DELTA;
	panoramaTime += DELTA;

	timeBuffer->GetData().time = min(lerpTime / PANORAMA_INTERVAL, 1.0f);

	if (panoramaTime >= PANORAMA_INTERVAL)
	{
		panoramaTime -= PANORAMA_INTERVAL;
		lerpTime -= PANORAMA_INTERVAL;

		curPanoramaIndex = (curPanoramaIndex + 1) % PANORAMA_SIZE;

		lobbyStart = lobbyEnd;

		string path = backgrounds[curPanoramaIndex];
		lobbyEnd = Texture::Add(Utility::ToWString(path));
	}

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
	timeBuffer->SetPS(10);

	lobbyStart->PSSet(10);
	lobbyEnd->PSSet(11);

	background->Render();
	title->Render();

	for (Button* button : buttons)
		button->Render();
}

void LobbyScene::GUIRender()
{
	//for (Button* button : buttons)
	//	button->Edit();
}

void LobbyScene::GameStart()
{
	ShowCursor(false);

	Audio::Get()->Stop("Title");

	BlockManager::Get()->CreateWorld();
	
	SceneManager::Get()->Add("InGame");
	SceneManager::Get()->Remove("Lobby");
}

void LobbyScene::LoadGame()
{
	ShowCursor(false);
	Audio::Get()->Stop("Title");

	BlockManager::Get()->Load();
	INVEN->Load();

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
