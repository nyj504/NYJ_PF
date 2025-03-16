#pragma once
class MainChunk
{
private:
    const int MAINCHUNK_HEIGHT = 64;
    const int SUBCHUNK_SIZE = 4;

public:
    MainChunk(Vector3 position, TerrainType terrainType, UINT64 myIndex);
    ~MainChunk();

    void Update();
    void Render();

    void GenerateTerrain();
    void MergeHeightMap(MainChunk* neighbor);
    void ActivateSubChunk();
    void SetInstanceData(bool isChange);
 
    Vector3 GetChunkPosition() { return chunkPosition; }

    SubChunk* GetActiveSubChunk();
  
    vector<InstanceData> GetTotalSingleInstanceDatas() { return totalSingleInstanceDatas; }
    vector<InstanceData> GetTotalMultiInstanceDatas() { return totalMultiInstanceDatas; }

    vector<SubChunk*>GetSubchunks() { return subChunks; }
private:
    Vector3 chunkPosition;

    UINT64 myIndex;
    TerrainType terrainType = TerrainType::PLAINS;
    int activeChunkIndex = 0;

    bool terrainGenerated = false;
    UINT heightMap[CHUNK_WIDTH + 1][CHUNK_DEPTH + 1];

    vector<SubChunk*>subChunks;
  
    pair<int, int>chunkIndex;

    vector<InstanceData> totalSingleInstanceDatas;
    vector<InstanceData> totalMultiInstanceDatas;

};