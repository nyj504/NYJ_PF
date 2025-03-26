#pragma once

class SubChunk
{
private:
	enum TreeType
	{
		CHAM, ACACIA
	};
public:
	SubChunk(int index, class WorldGenerator* worldGenerator);
	~SubChunk();

	void Update();
	void Render();

	Block* GetBlock(Vector3 globalPos);
	Block* GetSelectedBlock() { return selectedBlock; }
	
	void GenerateTerrain(Vector3 pos, UINT heightMap[CHUNK_WIDTH][CHUNK_DEPTH]);
	void GenerateTree(TreeType type, Vector3 pos);
	void CheckVisibleBlocks();
	void ActiveCollider();

	void SetParentIndex(UINT64 parentIndex) { this->parentIndex = parentIndex; }
	UINT64 GetParentIndex() { return parentIndex; }

	void MiningBlock(Block* block);
	void BuildBlock(Vector3 pos, int blockType);

	bool HasCollider() { return hasCollider; }
	bool IsMouseOverChunk() { return isMouseOver; }
	
	vector<InstanceData> GetVisibleSingleInstanceData() { return visibleSingleInstanceDatas; }
	vector<InstanceData> GetVisibleMultiInstanceData() { return visibleMultiInstanceDatas; }

private:
	void FindVisibleBlocks();

private:
	TreeType treeType = CHAM;
	bool isGenerateTree = false;
	bool hasCollider = false;
	bool isMouseOver = false;

	class WorldGenerator* worldGenerator;

	Vector3 worldPos;
	UINT64 parentIndex;
	UINT index;

	unordered_map<UINT64,Block*>blocks; 

	Block* selectedBlock = nullptr;

	vector<InstanceData> visibleSingleInstanceDatas;
	vector<InstanceData> visibleMultiInstanceDatas;
};