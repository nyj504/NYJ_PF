#pragma once
class EditScene : public Scene
{
public:
	EditScene();
	~EditScene();


	// Scene��(��) ���� ��ӵ�
	void Update() override;

	void PreRender() override;

	void Render() override;

	void PostRender() override;

	void GUIRender() override;

private:
	ObjectEditor* editor;
};