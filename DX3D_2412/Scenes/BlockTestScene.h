#pragma once
class BlockTestScene : public Scene
{
public:
	BlockTestScene();
	~BlockTestScene();

	// Scene��(��) ���� ��ӵ�
	void Update() override;

	void PreRender() override;

	void Render() override;

	void PostRender() override;

	void GUIRender() override;

private:
	HUDBar* hpBar;
	HUDBar* armorBar;
	HUDBar* hungerBar;
	Sky* skybox;

	Quad* item;

	Cube* cube;

	Monster* monster;
	Animal* animal;

	CrackEffect* effect;

	vector<Block*> blocks;
	vector<InstanceData> instanceData;
	VertexBuffer* instanceBuffer;
};