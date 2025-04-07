#pragma once
class InGameScene : public Scene
{
private:
	const float BGM_INTERVAL = 30.0f;

public:
	InGameScene();
	~InGameScene();

	// Scene을(를) 통해 상속됨
	void Update() override;

	void PreRender() override;

	void Render() override;

	void PostRender() override;

	void GUIRender() override;

	void ChangeBGM();
	void ChangeScene(bool isSceneActive) { this->isSceneActive = isSceneActive; }

private:
	float silenceTimer = 0.0f;
	bool isSceneActive = false;
	int musicIndex = 0;
	string curMusic = {};
	Sky* sky;

	bool isPaused = false;
};