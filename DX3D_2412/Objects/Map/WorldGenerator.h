#pragma once

class WorldGenerator
{
private:
	const int MAINCHUNK_HEIGHT = 64;
	const UINT MAX_MAINCHUNK = 10000;
	const int SURPLUS_SIZE = 10000;

public:
	WorldGenerator();
	~WorldGenerator();

	void Update();
	void Render();

	void CreateWorld();

	vector<MainChunk*> GetClosestMainChunks();

	void SetInstanceData(MainChunk* chunk, bool isChange);
	void AddInstanceData(Block* block, Vector3 position);
	void UpdateInstanceBuffer();

	void MiningBlock(Block* block);
	void BuildBlock(Vector3 pos, UINT key);

	vector<MainChunk*> GetChunksInRange(int distance);

	UINT GetBlockInstanceIndex() { return globalBlockIndex++; }
	void SetActiveSubChunk(SubChunk* subChunk) {activeSubChunk = subChunk;}

private:
	UINT globalBlockIndex = 0;

	TerrainType terrainType = TerrainType::PLAINS;

	unordered_map<UINT64, MainChunk*>mainChunks;

	Block* builtBlock = nullptr;

	vector<MainChunk*>closestChunks;

	SubChunk* activeSubChunk;

	Cube* singleFaceBlock;
	Cube* multiFaceBlock;

	unordered_map<UINT, InstanceData> totalSingleInstanceDatas;
	unordered_map<UINT, InstanceData> totalMultiInstanceDatas;

	VertexBuffer* singleInstanceBuffer;
	VertexBuffer* multiInstanceBuffer;
};