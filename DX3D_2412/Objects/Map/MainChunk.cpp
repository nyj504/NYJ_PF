#include "Framework.h"

MainChunk::MainChunk(Vector3 pos, TerrainType terrainType) : terrainType(terrainType)
{
    subChunks.reserve(SUBCHUNK_SIZE);
}

MainChunk::~MainChunk()
{
    for (SubChunk* subChunk : subChunks)
    {
        delete subChunk;
    }
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
        Vector3 subChunkPos = Vector3(pos.x, pos.y - (MAINCHUNK_HEIGHT)+(SUBCHUNK_HEIGHT * (i + 1)), pos.z);

        SubChunk* subChunk = new SubChunk(i);
        subChunk->SetParent(this);
        subChunk->GenerateTerrain(subChunkPos, heightMap);
        subChunk->UpdateWorld();

        subChunks.push_back(subChunk);
    }
}


void MainChunk::Update()
{
    for (SubChunk* subChunk : subChunks)
    {
        subChunk->Update();
    }
}

void MainChunk::Render()
{
    for (SubChunk* subChunk : subChunks)
    {
        subChunk->Render();
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
