#include "Framework.h"

MainChunk::MainChunk(Vector3 pos, TerrainType terrainType) : terrainType(terrainType)
{
    subChunks.reserve(SUBCHUNK_SIZE);
}

MainChunk::~MainChunk()
{
}

void MainChunk::Update()
{
    int playerY = PLAYER->GetGlobalPosition().y;
    int baseY = GetGlobalPosition().y; 

    activeChunkIndex = (baseY - playerY) / SUBCHUNK_HEIGHT;

    activeChunkIndex = max(0, min(activeChunkIndex, SUBCHUNK_SIZE - 1));

    if (!subChunks[activeChunkIndex]->HasCollider())
        subChunks[activeChunkIndex]->ActiveCollider();

    subChunks[activeChunkIndex]->Update();
}

void MainChunk::Render()
{
    if (subChunks[activeChunkIndex]->HasCollider())
    subChunks[activeChunkIndex]->Render();
}

void MainChunk::GenerateTerrain(Vector3 pos)
{
    PerlinNoise perlin((int)terrainType * 100);

    UINT heightMap[CHUNK_WIDTH +1][CHUNK_DEPTH +1];

    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int z = 0; z < CHUNK_DEPTH; z++)
        {
            float worldX = (pos.x + x) / 20.0f;
            float worldZ = (pos.z + z) / 20.0f;

            float baseHeight = 0.0f;
            float noiseFactor = 0.0f;

            switch (terrainType)
            {
            case TerrainType::PLAINS:
                baseHeight = 2.5f;
                noiseFactor = 2.5f;
                break;
            case TerrainType::HILLS:
                baseHeight = 8.5f;
                noiseFactor = 2.5f;
                break;
            case TerrainType::MOUNTAINS:
                baseHeight = 5.0f;
                noiseFactor = 11.0f;
                break;
            }

            float height = perlin.Noise(worldX, worldZ) * noiseFactor + baseHeight;
            heightMap[x][z] = (int)(height);
        }
    }

    for (int i = 0; i < SUBCHUNK_SIZE; i++)
    {
        Vector3 subChunkPos = Vector3(pos.x, pos.y - (SUBCHUNK_HEIGHT * i), pos.z);

        SubChunk* subChunk = new SubChunk(i);
        subChunk->SetParent(this);
        subChunk->GenerateTerrain(subChunkPos, heightMap);
        subChunk->UpdateWorld();

        subChunks.push_back(subChunk);
    }
}

void MainChunk::MergeHeightMap(MainChunk* neighborChunk)
{
    if (!neighborChunk) return;

    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int z = 0; z < CHUNK_DEPTH; z++)
        {
            int height = this->heightMap[x][z];
            int neighborHeight = neighborChunk->heightMap[x][z];

            if (abs(height - neighborHeight) > 3)
            {
                this->heightMap[x][z] = (height + neighborHeight) / 2;
            }
        }
    }
}

void MainChunk::ActivateSubChunk()
{
    activeChunkIndex = PLAYER->GetGlobalPosition().y / SUBCHUNK_HEIGHT;
}

void MainChunk::SetInstanceData()
{
    totalSingleInstanceDatas.clear(); 
    totalMultiInstanceDatas.clear();

    for (SubChunk* subChunk : subChunks)
    {
        subChunk->CheckVisibleBlocks();

        const vector<InstanceData>& newSingleInstanceData = subChunk->GetVisibleSingleInstanceData();
        const vector<InstanceData>& newMultiInstanceData = subChunk->GetVisibleMultiInstanceData();

        totalSingleInstanceDatas.insert(totalSingleInstanceDatas.end(), newSingleInstanceData.begin(), newSingleInstanceData.end());
        totalMultiInstanceDatas.insert(totalMultiInstanceDatas.end(), newMultiInstanceData.begin(), newMultiInstanceData.end());
    }
}

