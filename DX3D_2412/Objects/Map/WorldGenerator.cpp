#include "Framework.h"
#include "WorldGenerator.h"

WorldGenerator::WorldGenerator()
{
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
        Environment::Get()->SetAlphaBlend(true);
        singleInstanceBuffer->Set(1);
        singleFaceBlock->RenderInstanced(totalSingleInstanceDatas.size());
        Environment::Get()->SetAlphaBlend(false);
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
    int gridSize = 1; 
 
    for (int x = -gridSize; x <= gridSize; x++)
    {
        for (int z = -gridSize; z <= gridSize; z++)
        {
            float chunkX = x * CHUNK_WIDTH;
            float chunkZ = z * CHUNK_DEPTH;

            UINT64 chunkKey = GameMath::ChunkPosToKey(x, z);

            if (mainChunks.find(chunkKey) != mainChunks.end()) continue;

            TerrainType terrainType = TerrainType::PLAINS;

            Vector3 chunkPosition = { chunkX, 0, chunkZ };
            MainChunk* mainChunk = new MainChunk(chunkPosition, terrainType, chunkKey, this);

            mainChunk->GenerateTerrain();

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
        chunkOffsets =
        {
            {0, 0},   // 현재 청크
            {1, 0},   // 오른쪽 청크
            {0, 1},   // 위쪽 청크
            {1, 1}    // 오른쪽 위 대각선 청크
        };
    }
    else if (localX >= halfX && localZ < halfZ) // 오른쪽 아래 
    {
        chunkOffsets =
        {
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

    for (const pair<int, int>& offset : chunkOffsets)
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

void WorldGenerator::SetInstanceData(MainChunk* chunk, bool isChange)
{
    chunk->SetInstanceData(isChange);

    vector<InstanceData> newSingleInstanceData = chunk->GetTotalSingleInstanceDatas();
    vector<InstanceData> newMultiInstanceData = chunk->GetTotalMultiInstanceDatas();

    for (const auto& data : newSingleInstanceData)
    {
        if (data.isActive)
        {
            UINT index = data.index;
            totalSingleInstanceDatas[index] = data;
        }
    }

    for (const auto& data : newMultiInstanceData)
    {
        if (data.isActive)
        {
            UINT index = data.index;
            totalMultiInstanceDatas[index] = data;
        }
    }
}

void WorldGenerator::UpdateInstanceBuffer()
{
    vector<InstanceData>singleInstanceVec;
    vector<InstanceData>multiInstanceVec;

    singleInstanceVec.reserve(totalSingleInstanceDatas.size());
    multiInstanceVec.reserve(totalMultiInstanceDatas.size());

    for (const auto& pair : totalSingleInstanceDatas)
        singleInstanceVec.push_back(pair.second);

    for (const auto& pair : totalMultiInstanceDatas)
        multiInstanceVec.push_back(pair.second);

  
    if (singleInstanceBuffer)
    {
        delete singleInstanceBuffer; 
        singleInstanceBuffer = nullptr;
    }

    if (multiInstanceBuffer)
    {
        delete multiInstanceBuffer; 
        multiInstanceBuffer = nullptr;
    }

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
        singleInstanceBuffer->Update(singleInstanceVec.data(), (UINT)singleInstanceVec.size());
    }

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
        multiInstanceBuffer->Update(multiInstanceVec.data(), (UINT)multiInstanceVec.size());
    }
}

void WorldGenerator::MiningBlock(Block* block)
{
    if (!block) return;
    UINT blockID = block->GetBlockInstanceID();
    bool isNormal = block->IsNormal();

    if (isNormal)
    {
        totalSingleInstanceDatas.erase(blockID);
    }
    else
    {
        totalMultiInstanceDatas.erase(blockID);
    }

    activeSubChunk->FindVisibleBlocks();

    SetInstanceData(mainChunks[activeSubChunk->GetParentIndex()], true);

    UpdateInstanceBuffer();
    
}

void WorldGenerator::BuildBlock(Vector3 pos, UINT key)
{
    if (activeSubChunk)
    {
        activeSubChunk->BuildBlock(pos, key);
        SetInstanceData(mainChunks[activeSubChunk->GetParentIndex()], true);
        UpdateInstanceBuffer();
    }
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

void WorldGenerator::Save()
{
    string path = "Resources/Transforms/World.srt";
    BinaryWriter* writer = new BinaryWriter(path);

    int count = mainChunks.size();
    writer->Data<int>(count);

    for (const pair<UINT64, MainChunk*>& chunk : mainChunks)
    {
        writer->Data<UINT64>(chunk.second->GetMyIndex());
        writer->Data<Vector3>(chunk.second->GetChunkPosition());
        chunk.second->Save();
    }

    delete writer;
}

void WorldGenerator::Load()
{
    string path = "Resources/Transforms/World.srt";
    BinaryReader* reader = new BinaryReader(path);

    int count = reader->Data<int>();

    for (int i = 0; i < count; i++)
    {
        UINT64 chunkIndex = reader->Data<UINT64>();
        Vector3 chunkPosition = reader->Data<Vector3>();

        MainChunk* mainChunk = new MainChunk(chunkPosition, terrainType, chunkIndex, this);

        mainChunk->Load();

        mainChunks[chunkIndex] = mainChunk;
    }

    delete reader;
}

