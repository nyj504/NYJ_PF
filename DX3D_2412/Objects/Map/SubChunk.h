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
	void CheckVisibleBlocks();
	void ActiveCollider();
	Block* GetSelectedBlock();

	bool HasCollider() { return hasCollider; }
	
	vector<InstanceData> GetVisibleSingleInstanceData() { return visibleSingleInstanceDatas; }
	vector<InstanceData> GetVisibleMultiInstanceData() { return visibleMultiInstanceDatas; }

private:
	void FindSurroundedBlocks();

private:
	bool hasCollider = false;

	UINT index;

	unordered_map<UINT,Block>blocks; //ûũ �� ���
	vector<Block*>visibleBlocks; //ûũ �� ���̴� ���

	Block* selectedBlock = nullptr;

	vector<InstanceData> visibleSingleInstanceDatas;
	vector<InstanceData> visibleMultiInstanceDatas;
};