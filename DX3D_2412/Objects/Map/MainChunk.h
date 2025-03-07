#pragma once
class MainChunk : public Transform
{
private:
    const int MAINCHUNK_HEIGHT = 64;
    const int SUBCHUNK_SIZE = 4;

public:
    MainChunk(Vector3 pos, TerrainType terrainType);
    ~MainChunk();

    void GenerateTerrain(Vector3 pos);
    void Update();
    void Render();
    void MergeHeightMap(MainChunk* neighbor);
   
    SubChunk* GetSubChunkAt(int index)
    {
        if (index < 0 || index >= subChunks.size())
            return nullptr;

        return subChunks[index];
    }

    pair<int, int> GetIndex() const { return chunkIndex; }

private:
    TerrainType terrainType = TerrainType::PLAINS;

    bool terrainGenerated = false;
    UINT heightMap[CHUNK_WIDTH + 1][CHUNK_DEPTH + 1];

    vector<SubChunk*>subChunks;
  
    pair<int, int>chunkIndex;
};