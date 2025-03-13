#pragma once

class WorldGenerator
{
private:
	const int MAINCHUNK_HEIGHT = 64;
	const UINT MAX_MAINCHUNK = 10000;

public:
	WorldGenerator();
	~WorldGenerator();

	void Update();
	void Render();

	void CreateWorld();

	vector<MainChunk*> GetClosestMainChunks();

	void SetInstanceData(MainChunk* chunk);
	void UpdateInstanceBuffer();

	void ReserveInstanceData(size_t singleSize, size_t multiSize);

	vector<MainChunk*> GetChunksInRange(int distance);
private:
	TerrainType terrainType = TerrainType::PLAINS;

	unordered_map<UINT64, MainChunk*>mainChunks;
	vector<MainChunk*>closestChunks;

	Cube* singleFaceBlock;
	Cube* multiFaceBlock;

	vector<InstanceData> totalSingleInstanceDatas;
	vector<InstanceData> totalMultiInstanceDatas;

	VertexBuffer* singleInstanceBuffer;
	VertexBuffer* multiInstanceBuffer;

};