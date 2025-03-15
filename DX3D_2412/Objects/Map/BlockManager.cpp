#include "Framework.h"

BlockManager::BlockManager()
{
	worldGenerator = new WorldGenerator();

	lastPlayerPos = PLAYER->GetGlobalPosition();
	ActivateRenderingChunks();
	//Load();
}

BlockManager::~BlockManager()
{
	delete worldGenerator;
	for (const pair<UINT64, MainChunk*>& chunk : activeChunks)
	{
		delete chunk.second;
	}
	activeChunks.clear();
}

void BlockManager::Update()
{
	Vector3 currentPlayerPos = PLAYER->GetGlobalPosition();
	float distanceMoved = Vector3::Distance(currentPlayerPos, lastPlayerPos);
	
	worldGenerator->Update();
	
	if (distanceMoved >= updateThreshold)
	{
		lastPlayerPos = currentPlayerPos;
		ActivateRenderingChunks();
	}
}

void BlockManager::Render()
{
	worldGenerator->Render();
}

void BlockManager::PostRender()
{
}

void BlockManager::BuildBlock()
{
	if (!selectedBlock) return;

	pair<UINT, UINT>quickSlotData = QUICKSLOT->GetSelectedIndexData();
	
	if (quickSlotData.first == 0) return;

	ItemData data = DataManager::Get()->GetItemData(quickSlotData.first);
	
	if (!data.canBuild) return;

	Ray ray = CAM->ScreenPointToRay(mousePos);

	float minDistance = FLT_MAX;
	RaycastHit hit;

	if (selectedBlock->GetCollider()->IsRayCollision(ray, &hit))
	{
		Vector3 buildPosition = selectedBlock->GetGlobalPosition() + hit.normal;
		worldGenerator->BuildBlock(buildPosition, quickSlotData.first);
	}
}

void BlockManager::MiningBlock()
{
	if (!selectedBlock) return;
	
	selectedBlock->Damage();
	if (selectedBlock->GetHp() <= 0)
		worldGenerator->MiningBlock(selectedBlock);
}

void BlockManager::InteractingBlock()
{
	if (selectedBlock->GetBlockType() == 0) return;
	UIManager::Get()->SetTopSlot(selectedBlock->GetTag());
}

void BlockManager::ActivateCollisionBlocks()
{
	UINT updateDistance = 7;

	Vector3 playerPos = PLAYER->GetGlobalPosition();

	int chunkX = (int)(playerPos.x / CHUNK_WIDTH);
	int chunkZ = (int)(playerPos.z / CHUNK_DEPTH);

	for (int dx = 0; dx <= 1; dx++)
	{
		for (int dz = 0; dz <= 1; dz++)
		{
			UINT64 chunkKey = GameMath::ChunkPosToKey(chunkX + dx, chunkZ + dz);

			if (activeChunks.find(chunkKey) != activeChunks.end())	
			{
				MainChunk* mainChunk = activeChunks[chunkKey];
			}
		}
	}
}


void BlockManager::ActivateRenderingChunks()
{
	int renderDistance = 4; 

	activeChunks.clear();

	vector<MainChunk*> surroundingChunks = worldGenerator->GetChunksInRange(renderDistance);

	size_t totalSingleInstanceSize = 0;
	size_t totalMultiInstanceSize = 0;

	for (MainChunk* chunk : surroundingChunks)
	{
		UINT64 chunkKey = GameMath::ChunkPosToKey(chunk->GetGlobalPosition().x / CHUNK_WIDTH,
			chunk->GetGlobalPosition().z / CHUNK_DEPTH);
		chunk->SetInstanceData();
		
		totalSingleInstanceSize += chunk->GetTotalSingleInstanceDatas().size();
		totalMultiInstanceSize += chunk->GetTotalMultiInstanceDatas().size();

		activeChunks[chunkKey] = chunk;
	}

	worldGenerator->ReserveInstanceData(totalSingleInstanceSize, totalMultiInstanceSize);

	for (const pair<UINT64, MainChunk*>& chunk : activeChunks)
	{
		worldGenerator->SetInstanceData(chunk.second);
	}

	worldGenerator->UpdateInstanceBuffer();
}


//
//void BlockManager::Save()
//{
//	BinaryWriter* writer = new BinaryWriter("Resources/Transforms/Map.srt");
//
//	int count = blocks.size();
//	writer->Data<int>(count); 
//
//	int index = 0;
//
//	for (Block* block : blocks)
//	{
//		writer->Data<int>(index);
//		writer->Data<bool>(block->IsActive());
//		writer->Data<Vector3>(block->GetGlobalPosition());
//		writer->Data<Vector3>(block->GetGlobalRotation());
//		writer->Data<Vector3>(block->GetGlobalScale());
//		writer->Data<Vector3>(block->GetGlobalPivot());
//		writer->Data<UINT>(block->GetItemKey());
//
//		index++;
//	}
//	delete writer;
//}
//
//void BlockManager::Load()
//{
//	/*BinaryReader* reader = new BinaryReader("Resources/Transforms/Map.srt");
//
//	int blockCount = reader->Data<int>();
//	blocks.clear();
//	instanceDatas.clear();
//
//	blocks.reserve(blockCount);
//	instanceDatas.resize(blockCount);
//
//	for (int i = 0; i < blockCount; i++)
//	{
//		int index = reader->Data<int>(); 
//		bool isActive = reader->Data<bool>();
//		Vector3 position = reader->Data<Vector3>(); 
//		Vector3 rotation = reader->Data<Vector3>(); 
//		Vector3 scale = reader->Data<Vector3>();
//		Vector3 pivot = reader->Data<Vector3>();
//		UINT key = reader->Data<UINT>();
//		
//		Block* editBlock = nullptr;
//		ItemData data = DataManager::Get()->GetItemData(key);
//
//		if (data.textureType != "Model")
//			editBlock = new SimpleBlock(key);
//		else
//			editBlock = new ComplexBlock(key);
//
//		
//		editBlock->SetActive(isActive);
//		editBlock->SetLocalPosition(position);
//		editBlock->SetLocalRotation(rotation);
//		editBlock->SetLocalScale(scale);
//		editBlock->SetPivot(pivot);
//		editBlock->GetMaterial()->SetShader(L"Instancing/Instancing.hlsl");
//
//		blocks.emplace_back(editBlock);
//
//		instanceDatas[i].transform = XMMatrixTranslation(position.x, position.y, position.z);
//		instanceDatas[i].transform = XMMatrixTranspose(instanceDatas[i].transform);
//	}
//	UpdateInstanceBuffer();
//
//	delete reader;*/
//}



