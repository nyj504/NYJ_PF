#pragma once
struct pair_hash
{
	template <class T1, class T2>
	size_t operator()(const pair<T1, T2>& p) const
	{
		return hash<T1>{}(p.first) ^ (hash<T2>{}(p.second) << 1);
	}
};

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
	void UpdateChunks();
	void SetInstanceData(MainChunk* chunk);
	void UpdateInstanceBuffer();
	void ReserveInstanceData(size_t singleSize, size_t multiSize);

	vector<MainChunk*> GetChunksInRange(int distance);
private:
	TerrainType terrainType = TerrainType::PLAINS;

	unordered_map<UINT64, MainChunk*>mainChunks;
	queue<UINT> availableChunkIndices;  // 사용할 수 있는 인덱스 목록
	UINT nextChunkIndex = 0; 

	Cube* singleFaceBlock;
	Cube* multiFaceBlock;

	vector<InstanceData> totalSingleInstanceDatas;
	vector<InstanceData> totalMultiInstanceDatas;

	VertexBuffer* singleInstanceBuffer;
	VertexBuffer* multiInstanceBuffer;

};