#pragma once
class TestScene : public Scene
{
public:
	TestScene();
	~TestScene();

	// Scene을(를) 통해 상속됨
	void Update() override;

	void PreRender() override;

	void Render() override;

	void PostRender() override;

	void GUIRender() override;

	void Init();

private:
	Skybox* skybox;
	
	Monster* zombie;
	Animal* animal;

	bool isPaused = false;
};