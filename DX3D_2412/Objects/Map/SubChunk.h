#pragma once

class SubChunk : public Transform
{
public:
	SubChunk(int index);
	~SubChunk();

	void Update();
	void Render();

	Block* GetBlock(Vector3 globalPos);
	Block* GetSelectedBlock() { return selectedBlock; }
	
	void GenerateTerrain(Vector3 pos, UINT heightMap[CHUNK_WIDTH + 1][CHUNK_DEPTH + 1]);
	void CheckVisibleBlocks();
	void ActiveCollider();

	void SetParentIndex(UINT64 parentIndex) { this->parentIndex = parentIndex; }
	UINT64 GetParentIndex() { return parentIndex; }

	void MiningBlock(Block* block);
	void BuildBlock(Vector3 pos, int blockType);

	void CheckSelectedBlock();

	bool HasCollider() { return hasCollider; }
	
	vector<InstanceData> GetVisibleSingleInstanceData() { return visibleSingleInstanceDatas; }
	vector<InstanceData> GetVisibleMultiInstanceData() { return visibleMultiInstanceDatas; }

private:
	void FindVisibleBlocks();

private:
	bool hasCollider = false;
	Vector3 worldPos;
	UINT64 parentIndex;
	UINT index;

	unordered_map<UINT64,Block>blocks; //청크 내 블록
	vector<Block*>visibleBlocks; //청크 내 보이는 블록

	Block* selectedBlock = nullptr;

	vector<InstanceData> visibleSingleInstanceDatas;
	vector<InstanceData> visibleMultiInstanceDatas;
};