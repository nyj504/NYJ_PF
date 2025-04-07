#pragma once
class LobbyScene : public Scene
{
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

	void InitAudio();
private:
	Quad* title;
	Quad* background;
	vector<Button*>buttons;
};