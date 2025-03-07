#include "Framework.h"
#include "WorldGenerator.h"

WorldGenerator::WorldGenerator()
{
    CreateWorld();
}

WorldGenerator::~WorldGenerator()
{

}

void WorldGenerator::CreateWorld()
{
    int gridSize = 8; 
    PerlinNoise perlin(12345);

    for (int x = -gridSize; x <= gridSize; x++)
    {
        for (int z = -gridSize; z <= gridSize; z++)
        {
            float chunkX = x * CHUNK_WIDTH;
            float chunkZ = z * CHUNK_DEPTH;

            UINT64 chunkKey = GameMath::ChunkPosToKey(x, z);

            if (mainChunks.find(chunkKey) != mainChunks.end()) continue;

            float noiseValue = perlin.Noise(x * 0.1f, z * 0.1f) * 0.2f - 0.8f;  

            TerrainType terrainType = TerrainType::PLAINS;

            Vector3 chunkPosition = { chunkX, 0, chunkZ };
            MainChunk* mainChunk = new MainChunk(chunkPosition, terrainType);

            for (int dx = -1; dx <= 1; dx++)
            {
                for (int dz = -1; dz <= 1; dz++)
                {
                    if (dx == 0 && dz == 0) continue;

                    UINT64 neighborKey = GameMath::ChunkPosToKey(x + dx, z + dz);
                    if (mainChunks.find(neighborKey) != mainChunks.end())
                    {
                        MainChunk* neighborChunk = mainChunks[neighborKey];
                        mainChunk->MergeHeightMap(neighborChunk);
                    }
                }
            }

            mainChunk->GenerateTerrain(chunkPosition);
            mainChunk->SetLocalPosition(chunkPosition);
            mainChunk->UpdateWorld();

            mainChunks[chunkKey] = mainChunk;
        }
    }
}

void WorldGenerator::UpdateChunks()
{
    vector<MainChunk*> activeChunks = GetChunksInRange(2);

    for (MainChunk* chunk : activeChunks)
    {
        chunk->Update();
    }
}

vector<MainChunk*> WorldGenerator::GetChunksInRange(int renderDistance)
{
    Vector3 playerChunkPos = PLAYER->GetGlobalPosition() / CHUNK_WIDTH;

    vector<MainChunk*> surroundingChunks;
    surroundingChunks.reserve((renderDistance * 2 + 1)* (renderDistance * 2 + 1));

    for (int x = -renderDistance; x <= renderDistance; x++)
    {
        for (int z = -renderDistance; z <= renderDistance; z++)
        {
            UINT64 chunkKey = GameMath::ChunkPosToKey((int)playerChunkPos.x + x, (int)playerChunkPos.z + z);

            unordered_map<UINT64, MainChunk*>::iterator it = mainChunks.find(chunkKey);
            if (it != mainChunks.end())
            {
                surroundingChunks.push_back(it->second);
            }
        }
    }

    return surroundingChunks;
}
