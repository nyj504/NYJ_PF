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
	Cube* cube;
	Armor* armor;
	vector<Block*> blocks;
	vector<InstanceData> instanceData;
	VertexBuffer* instanceBuffer;
};