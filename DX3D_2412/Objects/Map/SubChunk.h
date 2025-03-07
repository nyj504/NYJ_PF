#pragma once

class SubChunk : public Transform
{
public:
	SubChunk(int index);
	~SubChunk();

	void Update();
	void Render();

	Block* GetBlock(Vector3 localPos);
	
	void GenerateTerrain(Vector3 pos, UINT heightMap[CHUNK_WIDTH + 1][CHUNK_DEPTH + 1]);
	void UpdateInstanceBuffer();
	void CheckVisibleBlocks();

private:
	void FindSurroundedBlocks();
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