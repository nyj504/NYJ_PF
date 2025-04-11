#pragma once
class WorldGenerator;

class MainChunk
{
private:
    const int MAINCHUNK_HEIGHT = 64;
    const int SUBCHUNK_SIZE = 4;

    const float PLAIN_HEIGHT = 2.5f;
    const float HILLS_HEIGHT = 6.5f;
    const float MOUNTAIN_HEIGHT = 5.0f;

    const float PLAIN_NOISE = 2.5f;
    const float HILLS_NOISE = 3.5f;
    const float MOUNTAIN_NOISE = 5.5f;

public:
    MainChunk(Vector3 position, TerrainType terrainType, UINT64 myIndex, WorldGenerator* worldGenerator);
    ~MainChunk();

    void Update();
    void Render();

    void GenerateTerrain();
    void SetInstanceData(bool isChange);
 
    Vector3 GetChunkPosition() { return chunkPosition; }
  
    vector<InstanceData> GetTotalSingleInstanceDatas() { return totalSingleInstanceDatas; }
    vector<InstanceData> GetTotalMultiInstanceDatas() { return totalMultiInstanceDatas; }

    vector<SubChunk*>GetSubchunks() { return subChunks; }
    UINT64 GetMyIndex() { return myIndex; }

    void Save();
    void Load();

private:
    Vector3 chunkPosition;

    UINT64 myIndex;
    TerrainType terrainType = TerrainType::PLAINS;
    int activeChunkIndex = 0;

    bool terrainGenerated = false;

    vector<SubChunk*>subChunks;
  
    pair<int, int>chunkIndex;

    vector<InstanceData> totalSingleInstanceDatas;
    vector<InstanceData> totalMultiInstanceDatas;

    WorldGenerator* worldGenerator;
};