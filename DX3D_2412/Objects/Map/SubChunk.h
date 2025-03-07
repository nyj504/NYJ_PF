#pragma once

class SubChunk : public Transform
{
public:
	SubChunk(int index);
	~SubChunk();

	void Update();
	void Render();

	Block* GetBlock(int x, int y, int z);
	void GenerateTerrain(Vector3 pos, UINT heightMap[CHUNK_WIDTH + 1][CHUNK_DEPTH + 1]);
	void UpdateInstanceBuffer();

private:
	void FindSurroundedBlocks();
	void CheckVisibleBlocks();
private:
	UINT index;

	unordered_map<UINT,Block>blocks; //ûũ �� ���
	vector<Block*>visibleBlocks; //ûũ �� ���̴� ���

	Cube* singleFaceBlock;
	Cube* multiFaceBlock;

	vector<InstanceData> visibleSingleInstanceDatas;
	vector<InstanceData> visibleMultiInstanceDatas;

	VertexBuffer* singleInstanceBuffer;
	VertexBuffer* multiInstanceBuffer;

};