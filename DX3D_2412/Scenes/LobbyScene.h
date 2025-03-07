#pragma once
class LobbyScene : public Scene
{
public:
	LobbyScene();
	~LobbyScene();

	// Scene��(��) ���� ��ӵ�
	void Update() override;

	void PreRender() override;

	void Render() override;

	void PostRender() override;

	void GUIRender() override;

	void GameStart();
private:
	Quad* title;
	Quad* background;
	vector<Button*>buttons;
};