#pragma once
class TestScene : public Scene
{
public:
	TestScene();
	~TestScene();

	// Scene��(��) ���� ��ӵ�
	void Update() override;

	void PreRender() override;

	void Render() override;

	void PostRender() override;

	void GUIRender() override;

private:
	Sky* sky;
	
	Monster* zombie;
	Animal* animal;

	bool isPaused = false;
};