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
    int minBaseY = chunkPosition.y;
    
    activeChunkIndex = (abs(playerY - minBaseY) / SUBCHUNK_HEIGHT) % SUBCHUNK_SIZE;

    if (!subChunks[activeChunkIndex]->HasCollider())
    {
        subChunks[activeChunkIndex]->ActiveCollider();
        subChunks[1]->ActiveCollider();
    }

     subChunks[activeChunkIndex]->Update();
     subChunks[activeChunkIndex + 1]->Update();
    
}

void MainChunk::Render()
{
    subChunks[activeChunkIndex]->Render();
    subChunks[activeChunkIndex + 1]->Render();
}

void MainChunk::GenerateTerrain()
{
    PerlinNoise perlin;

    UINT heightMap[CHUNK_WIDTH][CHUNK_DEPTH];

    for (int x = 0; x < CHUNK_WIDTH; x++)
    {
        for (int z = 0; z < CHUNK_DEPTH; z++)
        {
            float worldX = (chunkPosition.x + x) / 20.0f;
            float worldZ = (chunkPosition.z + z) / 20.0f;

            float baseHeight = 0.0f; //��ǥ�� ����ûũ�� �⺻ ����
            float noiseFactor = 0.0f; // �޸� ������ ��鸲

            switch (terrainType)
            {
            case TerrainType::PLAINS:
                baseHeight = PLAIN_HEIGHT;
                noiseFactor = PLAIN_NOISE;
                break;
            case TerrainType::HILLS:
                baseHeight = HILLS_HEIGHT;
                noiseFactor = HILLS_NOISE;
                break;
            case TerrainType::MOUNTAINS:
                baseHeight = MOUNTAIN_HEIGHT;
                noiseFactor = MOUNTAIN_NOISE;
                break;
            }
            float height = perlin.Noise(worldX, worldZ) * noiseFactor + baseHeight;

            if (terrainType == TerrainType::HILLS || terrainType == TerrainType::MOUNTAINS)
            {
                float plainBase = PLAIN_HEIGHT;
                float plainNoise = PLAIN_NOISE;

                float plainHeight = perlin.Noise(worldX, worldZ) * plainNoise + plainBase;

                // ���� ��ġ �������� Chunk �߽ɿ��� �󸶳� ���������� ���
                float localX = (float)x / (float)(CHUNK_WIDTH - 1);
                float localZ = (float)z / (float)(CHUNK_DEPTH - 1);

                // �����ڸ������� 0 �� �߾����� ������ 1 (�ձ۰� ó���ϸ� �� �ε巯��)
                float edgeFactorX = 1.0f - abs(localX - 0.5f) * 2.0f;
                float edgeFactorZ = 1.0f - abs(localZ - 0.5f) * 2.0f;

                float edgeBlendFactor = min(edgeFactorX, edgeFactorZ); // �𼭸��� ���� ���� blending
                edgeBlendFactor = min(max(edgeBlendFactor, 0.0f), 1.0f); // clamp

                // �߽ɿ��� 1 �� �𼭸��� 0 ���� �ڿ������� blending
                height = GameMath::Lerp(plainHeight, height, edgeBlendFactor);
            }

            heightMap[x][z] = (int)(height); //����ûũ�� ���̸� ���� 
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
            subChunk->FindVisibleBlocks();
        }
       
        vector<InstanceData> newSingleInstanceData = subChunk->GetVisibleSingleInstanceData();
        vector<InstanceData> newMultiInstanceData = subChunk->GetVisibleMultiInstanceData();

        totalSingleInstanceDatas.insert(totalSingleInstanceDatas.end(), newSingleInstanceData.begin(), newSingleInstanceData.end());
        totalMultiInstanceDatas.insert(totalMultiInstanceDatas.end(), newMultiInstanceData.begin(), newMultiInstanceData.end());
    }
}

void MainChunk::Save()
{
    string path = "Resources/Transforms/Map" + to_string(myIndex) + ".srt";
    BinaryWriter* writer = new BinaryWriter(path);

    writer->Data<Vector3>(chunkPosition);
   
    for (SubChunk* subChunk : subChunks)
    {
        subChunk->Save();
    }

    delete writer;
}

void MainChunk::Load()
{
    string path = "Resources/Transforms/Map" + to_string(myIndex) + ".srt";
    BinaryReader* reader = new BinaryReader(path);

    Vector3 mainchunkPosition = reader->Data<Vector3>();
   
    for (int i = 0; i < SUBCHUNK_SIZE; i++)
    {
        Vector3 subChunkPos = Vector3(mainchunkPosition.x, chunkPosition.y - (SUBCHUNK_HEIGHT * i), mainchunkPosition.z);

        SubChunk* subChunk = new SubChunk(i, worldGenerator);
        subChunk->SetParentIndex(myIndex);
        subChunk->Load();

        subChunks.push_back(subChunk);
    }

    delete reader;
}
