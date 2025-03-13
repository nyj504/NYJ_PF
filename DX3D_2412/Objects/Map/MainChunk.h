#pragma once
class MainChunk : public Transform
{
private:
    const int MAINCHUNK_HEIGHT = 64;
    const int SUBCHUNK_SIZE = 4;

public:
    MainChunk(Vector3 pos, TerrainType terrainType);
    ~MainChunk();

    void Update();
    void Render();

    void GenerateTerrain(Vector3 pos);
    void MergeHeightMap(MainChunk* neighbor);
    void ActivateSubChunk();
    void SetInstanceData();
  
    vector<InstanceData> GetTotalSingleInstanceDatas() { return totalSingleInstanceDatas; }
    vector<InstanceData> GetTotalMultiInstanceDatas() { return totalMultiInstanceDatas; }

    vector<SubChunk*>GetSubchunks() { return subChunks; }

    pair<int, int> GetIndex() const { return chunkIndex; }

private:
    TerrainType terrainType = TerrainType::PLAINS;
    int activeChunkIndex = 0;

    bool terrainGenerated = false;
    UINT heightMap[CHUNK_WIDTH + 1][CHUNK_DEPTH + 1];

    vector<SubChunk*>subChunks;
  
    pair<int, int>chunkIndex;

    vector<InstanceData> totalSingleInstanceDatas;
    vector<InstanceData> totalMultiInstanceDatas;

};