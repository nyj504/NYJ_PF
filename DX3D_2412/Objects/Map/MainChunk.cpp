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

void MainChunk::CheckVisibleBlock()
{
    for (SubChunk* subChunk : subChunks)
    {
        subChunk->CheckVisibleBlocks();
    }
}

vector<Block*> MainChunk::GetCollidableBlocks(UINT range)
{
    vector<Block*> collidableBlocks;

    Vector3 playerPos = PLAYER->GetGlobalPosition();

    float chunkMinX = GetGlobalPosition().x;
    float chunkMaxX = chunkMinX + CHUNK_WIDTH;
    float chunkMinZ = GetGlobalPosition().z;
    float chunkMaxZ = chunkMinZ + CHUNK_DEPTH;

    if (playerPos.x + range < chunkMinX || playerPos.x - range >= chunkMaxX ||
        playerPos.z + range < chunkMinZ || playerPos.z - range >= chunkMaxZ)
    {
        return collidableBlocks;
    }

    int startX = max((int)chunkMinX, (int)playerPos.x - range);
    int startY = (int)playerPos.y - range;
    int startZ = max((int)chunkMinZ, (int)playerPos.z - range);

    int endX = min((int)chunkMaxX - 1, (int)playerPos.x + range);
    int endY = (int)playerPos.y + range;
    int endZ = min((int)chunkMaxZ - 1, (int)playerPos.z + range);

    for (SubChunk* subChunk : subChunks)
    {
        for (int x = startX; x <= endX; x++)
        {
            for (int y = startY; y <= endY; y++)
            {
                for (int z = startZ; z <= endZ; z++)
                {
                    int localX = x % CHUNK_WIDTH;
                    int localZ = z % CHUNK_DEPTH;
                    int localY = y % SUBCHUNK_HEIGHT;

                    if (localX < 0) localX += CHUNK_WIDTH;
                    if (localZ < 0) localZ += CHUNK_DEPTH;
                    if (localY < 0) localY += SUBCHUNK_HEIGHT;

                    int subChunkIndex = (y - ((int)GetGlobalPosition().y - MAINCHUNK_HEIGHT)) / SUBCHUNK_HEIGHT;

                    if (subChunkIndex < 0 || subChunkIndex >= subChunks.size())
                        continue;

                    SubChunk* targetSubChunk = subChunks[subChunkIndex - 1];
                    Block* block = targetSubChunk->GetBlock(localX, localY, localZ);

                    if (block)
                        collidableBlocks.push_back(block);  
                }
            }
        }
    }

    return collidableBlocks;
}
