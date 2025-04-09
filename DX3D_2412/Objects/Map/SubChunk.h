#pragma once

class SubChunk
{
private:
	enum TreeType
	{
		CHAM, ACACIA
	};
	const int INTERACTABLE_DISTANCE = 5;

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
	
	vector<InstanceData> GetVisibleSingleInstanceData() { return visibleSingleInstanceDatas; }
	vector<InstanceData> GetVisibleMultiInstanceData() { return visibleMultiInstanceDatas; }

	void FindVisibleBlocks();

	void Save();
	void Load();

private:
	TreeType treeType = CHAM;
	bool hasCollider = false;

	class WorldGenerator* worldGenerator;

	Vector3 worldPos;
	UINT64 parentIndex;
	UINT index;
	UINT mainchunkIndex;

	unordered_map<UINT64,Block*>blocks; 

	Block* selectedBlock = nullptr;
	Block* steppedBlock = nullptr;

	vector<InstanceData> visibleSingleInstanceDatas;
	vector<InstanceData> visibleMultiInstanceDatas;
};