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

	void GenerateTerrain(Vector3 pos, UINT heightMap[CHUNK_WIDTH][CHUNK_DEPTH]);
	void GenerateTree(TreeType type, Vector3 pos);

	void CheckPlayerCollision();
	void CheckMonsterCollision();

	void ActiveCollider();
	void Test();

	void SetParentIndex(UINT64 parentIndex) { this->parentIndex = parentIndex; }
	UINT64 GetParentIndex() { return parentIndex; }

	void BuildBlock(Vector3 pos, int blockType);

	bool HasCollider() { return hasCollider; }
	bool IsMouseOverChunk() { return isMouseOver; }

	vector<InstanceData> GetVisibleSingleInstanceData() { return visibleSingleInstanceDatas; }
	vector<InstanceData> GetVisibleMultiInstanceData() { return visibleMultiInstanceDatas; }

	void FindVisibleBlocks();

private:
	TreeType treeType = CHAM;
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