#include "Framework.h"

MainChunk::MainChunk(Vector3 position, TerrainType terrainType, UINT64 myIndex, WorldGenerator* worldGenerator)
    : chunkPosition(position), terrainType(terrainType), myIndex(myIndex), worldGenerator(worldGenerator)
{
    subChunks.reserve(SUBCHUNK_SIZE);
    totalSingleInstanceDatas.reserve(SUBCHUNK_SIZE * CHUNK_WIDTH * CHUNK_DEPTH * SUBCHUNK_HEIGHT);
    totalMultiInstanceDatas.reserve(SUBCHUNK_SIZE * CHUNK_WIDTH * CHUNK_DEPTH * SUBCHUNK_HEIGHT);
}

MainChunk::~MainChunk()
{
    for (SubChunk* chunk : subChunks)
        delete chunk;
}

void MainChunk::Update()
{
    int playerY = PLAYER->GetGlobalPosition().y;
    int baseY = chunkPosition.y;

    activeChunkIndex = abs((baseY - playerY) / SUBCHUNK_HEIGHT);

    if (!subChunks[activeChunkIndex]->HasCollider())
        subChunks[activeChunkIndex]->ActiveCollider();

     subChunks[activeChunkIndex]->Update();
    
    for (SubChunk* chunk : subChunks)
    {
        if (chunk->IsMouseOverChunk())
        {
            BlockManager::Get()->SetSelectedBlock(subChunks[activeChunkIndex]->GetSelectedBlock());
        }
    }
}

void MainChunk::Render()
{
    if (subChunks[activeChunkIndex]->HasCollider())
    subChunks[activeChunkIndex]->Render();
}

void MainChunk::GenerateTerrain()
{
    PerlinNoise perlin((int)terrainType * 100);

    UINT heightMap[CHUNK_WIDTH][CHUNK_DEPTH];

    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int z = 0; z < CHUNK_DEPTH; z++)
        {
            float worldX = (chunkPosition.x + x) / 20.0f;
            float worldZ = (chunkPosition.z + z) / 20.0f;

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
        Vector3 subChunkPos = Vector3(chunkPosition.x, chunkPosition.y - (SUBCHUNK_HEIGHT * i), chunkPosition.z);

        SubChunk* subChunk = new SubChunk(i, worldGenerator);
        subChunk->SetParentIndex(myIndex);
        subChunk->GenerateTerrain(subChunkPos, heightMap);

        subChunks.push_back(subChunk);
    }
}


void MainChunk::SetInstanceData(bool isChange)
{
    totalSingleInstanceDatas.clear(); 
    totalMultiInstanceDatas.clear();

    for (SubChunk* subChunk : subChunks)
    {
        if (!isChange)
        {
            subChunk->CheckVisibleBlocks();
        }
       
        vector<InstanceData> newSingleInstanceData = subChunk->GetVisibleSingleInstanceData();
        vector<InstanceData> newMultiInstanceData = subChunk->GetVisibleMultiInstanceData();

        totalSingleInstanceDatas.insert(totalSingleInstanceDatas.end(), newSingleInstanceData.begin(), newSingleInstanceData.end());
        totalMultiInstanceDatas.insert(totalMultiInstanceDatas.end(), newMultiInstanceData.begin(), newMultiInstanceData.end());
    }
}
