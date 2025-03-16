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

	void SetInstanceData(MainChunk* chunk);
	void UpdateInstanceBuffer();

	void MiningBlock(Block* block);
	void BuildBlock(Vector3 pos, UINT index);

	void ReserveInstanceData(UINT singleSize, UINT multiSize);

	vector<MainChunk*> GetChunksInRange(int distance);
private:
	TerrainType terrainType = TerrainType::PLAINS;

	unordered_map<UINT64, MainChunk*>mainChunks;

	Block* builtBlock = nullptr;

	vector<MainChunk*>closestChunks;
	UINT test;

	SubChunk* activeSubChunk;

	vector<InstanceData> singleInstanceVec;
	vector<InstanceData> multiInstanceVec;

	Cube* singleFaceBlock;
	Cube* multiFaceBlock;

	unordered_map<UINT64, InstanceData> totalSingleInstanceDatas;
	unordered_map<UINT64, InstanceData> totalMultiInstanceDatas;

	VertexBuffer* singleInstanceBuffer;
	VertexBuffer* multiInstanceBuffer;

};