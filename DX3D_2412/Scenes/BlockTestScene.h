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

	Cube* cube;
	vector<Block*> blocks;
	vector<InstanceData> instanceData;
	VertexBuffer* instanceBuffer;
};