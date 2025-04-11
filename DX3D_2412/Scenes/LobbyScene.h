#pragma once
class LobbyScene : public Scene
{
private:
	const int PANORAMA_SIZE = 4;
	const float PANORAMA_INTERVAL = 20.0f;
public:
	LobbyScene();
	~LobbyScene();

	// Scene을(를) 통해 상속됨
	void Update() override;

	void PreRender() override;

	void Render() override;

	void PostRender() override;

	void GUIRender() override;

	void GameStart();
	void LoadGame();

	void InitAudio();
private:
	int curPanoramaIndex = 1;
	float lerpTime = 0.0f;
	float panoramaTime = 0.0f;

	TimeBuffer* timeBuffer;

	Texture* lobbyStart;
	Texture* lobbyEnd;

	unordered_map<UINT, string>backgrounds;
	
	Quad* title;
	Quad* background;
	
	vector<Button*>buttons;
};