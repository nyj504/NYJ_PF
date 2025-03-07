#pragma once
class EditScene : public Scene
{
public:
	EditScene();
	~EditScene();


	// Scene을(를) 통해 상속됨
	void Update() override;

	void PreRender() override;

	void Render() override;

	void PostRender() override;

	void GUIRender() override;

private:
	ObjectEditor* editor;
};