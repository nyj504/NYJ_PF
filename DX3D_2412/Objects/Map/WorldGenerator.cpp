#include "Framework.h"
#include "WorldGenerator.h"

WorldGenerator::WorldGenerator()
{
    CreateWorld();

    singleFaceBlock = new Cube();
    singleFaceBlock->GetMaterial()->SetDiffuseMap(L"Resources/Textures/BlockTexture/Atlasmap.png");
    singleFaceBlock->GetMaterial()->SetShader(L"Instancing/Instancing.hlsl");

    multiFaceBlock = new Cube();
    multiFaceBlock->ApplyObjectUVMapping();
    multiFaceBlock->GetMaterial()->SetDiffuseMap(L"Resources/Textures/BlockTexture/Atlasmap2.png");
    multiFaceBlock->GetMaterial()->SetShader(L"Instancing/MultiFaceInstancing.hlsl");
}

WorldGenerator::~WorldGenerator()
{
    delete singleFaceBlock;
    delete multiFaceBlock;

    if (singleInstanceBuffer)
        delete singleInstanceBuffer;

    if (multiInstanceBuffer)
        delete multiInstanceBuffer;
}

void WorldGenerator::Update()
{
}

void WorldGenerator::Render()
{
    if (!singleInstanceBuffer && !multiInstanceBuffer) return;

    if (singleInstanceBuffer)
    {
        singleInstanceBuffer->Set(1);
        UINT size = totalSingleInstanceDatas.size();
        singleFaceBlock->RenderInstanced(totalSingleInstanceDatas.size());
    }

    if (multiInstanceBuffer)
    {
        multiInstanceBuffer->Set(1);
        multiFaceBlock->RenderInstanced(totalMultiInstanceDatas.size());
    }
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
}

void WorldGenerator::SetInstanceData(MainChunk* chunk)
{
    vector<InstanceData> newSingleInstanceData = chunk->GetTotalSingleInstanceDatas();
    vector<InstanceData> newMultiInstanceData = chunk->GetTotalMultiInstanceDatas();

    totalSingleInstanceDatas.insert(totalSingleInstanceDatas.end(), newSingleInstanceData.begin(), newSingleInstanceData.end());
    totalMultiInstanceDatas.insert(totalMultiInstanceDatas.end(), newMultiInstanceData.begin(), newMultiInstanceData.end());
}

void WorldGenerator::UpdateInstanceBuffer()
{
    if (totalSingleInstanceDatas.empty())
    {
        if (singleInstanceBuffer)
        {
            delete singleInstanceBuffer;
            singleInstanceBuffer = nullptr;
        }
    }
    else
    {
        if (!singleInstanceBuffer)
        {
            singleInstanceBuffer = new VertexBuffer(
                totalSingleInstanceDatas.data(),
                sizeof(InstanceData),
                (UINT)totalSingleInstanceDatas.size()
            );
        }
        else
        {
            singleInstanceBuffer->Update(
                totalSingleInstanceDatas.data(),
                sizeof(InstanceData) * (UINT)totalSingleInstanceDatas.size()
            );
        }
    }
    if (totalMultiInstanceDatas.empty())
    {
        if (multiInstanceBuffer)
        {
            delete multiInstanceBuffer;
            multiInstanceBuffer = nullptr;
        }
    }
    else
    {
        if (!multiInstanceBuffer)
        {
            multiInstanceBuffer = new VertexBuffer(
                totalMultiInstanceDatas.data(),
                sizeof(InstanceData),
                (UINT)totalMultiInstanceDatas.size()
            );
        }
        else
        {
            multiInstanceBuffer->Update(
                totalMultiInstanceDatas.data(),
                sizeof(InstanceData) * (UINT)totalMultiInstanceDatas.size()
            );
        }
    }
}

void WorldGenerator::ReserveInstanceData(size_t singleSize, size_t multiSize)
{
    totalSingleInstanceDatas.clear();
    totalMultiInstanceDatas.clear();

    totalSingleInstanceDatas.reserve(singleSize);
    totalMultiInstanceDatas.reserve(multiSize);
}

vector<MainChunk*> WorldGenerator::GetChunksInRange(int distance)
{
    Vector3 playerChunkPos = PLAYER->GetGlobalPosition() / CHUNK_WIDTH;

    vector<MainChunk*> surroundingChunks;
    surroundingChunks.reserve((distance * 2 + 1)* (distance * 2 + 1));

    for (int x = -distance; x <= distance; x++)
    {
        for (int z = -distance; z <= distance; z++)
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
