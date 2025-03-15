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

    for (const pair<UINT64, MainChunk*>& chunk : mainChunks)
    {
        delete chunk.second;
    }
}

void WorldGenerator::Update()
{
    closestChunks = GetClosestMainChunks();

    if (!closestChunks.empty())
    {
        for (MainChunk* chunk : closestChunks)
        {
            chunk->Update();
        }
    }
}

void WorldGenerator::Render()
{
    if (!singleInstanceBuffer && !multiInstanceBuffer) return;

    if (singleInstanceBuffer)
    {
        singleInstanceBuffer->Set(1);
        singleFaceBlock->RenderInstanced(totalSingleInstanceDatas.size());
    }

    if (multiInstanceBuffer)
    {
        multiInstanceBuffer->Set(1);
        multiFaceBlock->RenderInstanced(totalMultiInstanceDatas.size());
    }

    if (!closestChunks.empty())
    {
        for (MainChunk* chunk : closestChunks)
        {
            chunk->Render();
        }
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
            MainChunk* mainChunk = new MainChunk(chunkPosition, terrainType, chunkKey);

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

vector<MainChunk*> WorldGenerator::GetClosestMainChunks()
{
    vector<MainChunk*> closestChunks;

    float playerX = PLAYER->GetGlobalPosition().x;
    float playerZ = PLAYER->GetGlobalPosition().z;

    int chunkX = (int)(floor(playerX / CHUNK_WIDTH));
    int chunkZ = (int)(floor(playerZ / CHUNK_DEPTH));

    float localX = playerX - (chunkX * CHUNK_WIDTH);
    float localZ = playerZ - (chunkZ * CHUNK_DEPTH);

    float halfX = CHUNK_WIDTH / 2;
    float halfZ = CHUNK_DEPTH / 2;

    vector<pair<int, int>> chunkOffsets;

    if (localX >= halfX && localZ > halfZ) // 오른쪽 위
    {
        chunkOffsets = {
            {0, 0},   // 현재 청크
            {1, 0},   // 오른쪽 청크
            {0, 1},   // 위쪽 청크
            {1, 1}    // 오른쪽 위 대각선 청크
        };
    }
    else if (localX >= halfX && localZ < halfZ) // 오른쪽 아래 
    {
        chunkOffsets = {
            {0, 0},   // 현재 청크
            {1, 0},   // 오른쪽 청크
            {0, -1},  // 아래쪽 청크
            {1, -1}   // 오른쪽 아래 대각선 청크
        };
    }
    else if (localX < halfX && localZ > halfZ) // 왼쪽 위
    {
        chunkOffsets = {
            {0, 0},   // 현재 청크
            {-1, 0},  // 왼쪽 청크
            {0, 1},   // 위쪽 청크
            {-1, 1}   // 왼쪽 위 대각선 청크
        };
    }
    else // (localX < 0.5f && localZ < 0.5f) // 왼쪽 아래
    {
        chunkOffsets = {
            {0, 0},   // 현재 청크
            {-1, 0},  // 왼쪽 청크
            {0, -1},  // 아래쪽 청크
            {-1, -1}  // 왼쪽 아래 대각선 청크
        };
    }

    for (const auto& offset : chunkOffsets)
    {
        int targetX = chunkX + offset.first;
        int targetZ = chunkZ + offset.second;

        UINT64 chunkKey = GameMath::ChunkPosToKey(targetX, targetZ);

        auto it = mainChunks.find(chunkKey);
        if (it != mainChunks.end())
        {
            closestChunks.push_back(it->second);
        }
    }

    return closestChunks;
}

void WorldGenerator::SetInstanceData(MainChunk* chunk)
{
    vector<InstanceData> newSingleInstanceData = chunk->GetTotalSingleInstanceDatas();
    vector<InstanceData> newMultiInstanceData = chunk->GetTotalMultiInstanceDatas();

    for (const auto& data : newSingleInstanceData)
    {
        UINT64 id = data.blockID; 
        totalSingleInstanceDatas[id] = data;
    }

    for (const auto& data : newMultiInstanceData)
    {
        UINT64 id = data.blockID;
        totalMultiInstanceDatas[id] = data;
    }
}

void WorldGenerator::UpdateInstanceBuffer()
{
    // 1. unordered_map → vector 변환
    vector<InstanceData> singleInstanceVec;
    vector<InstanceData> multiInstanceVec;

    for (const auto& pair : totalSingleInstanceDatas)
        singleInstanceVec.push_back(pair.second);

    for (const auto& pair : totalMultiInstanceDatas)
        multiInstanceVec.push_back(pair.second);

    // 2. GPU 버퍼 업데이트
    if (singleInstanceVec.empty())
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
                singleInstanceVec.data(),
                sizeof(InstanceData),
                (UINT)singleInstanceVec.size()
            );
        }
        else
        {
            singleInstanceBuffer->Update(
                singleInstanceVec.data(),
                sizeof(InstanceData) * (UINT)singleInstanceVec.size()
            );
        }
    }

    if (multiInstanceVec.empty())
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
                multiInstanceVec.data(),
                sizeof(InstanceData),
                (UINT)multiInstanceVec.size()
            );
        }
        else
        {
            multiInstanceBuffer->Update(
                multiInstanceVec.data(),
                sizeof(InstanceData) * (UINT)multiInstanceVec.size()
            );
        }
    }
}

void WorldGenerator::MiningBlock(Block* block)
{
    if (!block) return;

    UINT64 blockID = block->GetBlockID();

    if (block->IsNormal())
    {
        if (totalSingleInstanceDatas.count(blockID))
            totalSingleInstanceDatas.erase(blockID);
    }
    else
    {
        if (totalMultiInstanceDatas.count(blockID))
            totalMultiInstanceDatas.erase(blockID);
    }

    UpdateInstanceBuffer();
}

void WorldGenerator::BuildBlock(Vector3 pos, UINT index)
{
    if (!closestChunks.empty())
    {
        activeSubChunk = mainChunks[BlockManager::Get()->GetSelectedBlock()->GetParentIndex()]->GetActiveSubChunk();
    }

    activeSubChunk->BuildBlock(pos, index);
    builtBlock = activeSubChunk->GetBlock(pos);

    InstanceData instanceData;

    UVInfo uvInfo = builtBlock->GetUVInfo();
    UINT64 blockID = builtBlock->GetBlockID();

    Vector3 blockPos = builtBlock->GetGlobalPosition();

    instanceData.transform = XMMatrixTranslation(blockPos.x, blockPos.y, blockPos.z);
    instanceData.transform = XMMatrixTranspose(instanceData.transform);
        
    instanceData.curFrame = uvInfo.uvStart;
    instanceData.maxFrame = uvInfo.uvEnd;

    instanceData.blockID = builtBlock->GetBlockID();

    if (builtBlock->IsNormal())
    {
        totalSingleInstanceDatas[blockID] = instanceData;
    }
    else
    {
        totalMultiInstanceDatas[blockID] = instanceData;
    }

    UpdateInstanceBuffer();
}

void WorldGenerator::ReserveInstanceData(size_t singleSize, size_t multiSize)
{
    totalSingleInstanceDatas.clear();
    totalMultiInstanceDatas.clear();

    totalSingleInstanceDatas.reserve(singleSize + SURPLUS_SIZE);
    totalMultiInstanceDatas.reserve(multiSize + SURPLUS_SIZE);
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
