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

	void CreateWorld();
	void UpdateChunks();

	vector<MainChunk*> GetChunksInRange(int distance);
private:
	TerrainType terrainType = TerrainType::PLAINS;

	unordered_map<UINT64, MainChunk*>mainChunks;
	queue<UINT> availableChunkIndices;  // 사용할 수 있는 인덱스 목록
	UINT nextChunkIndex = 0; 
};