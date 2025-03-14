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
	//for (Block* block : blocks)
	//	delete block;
	//
	//delete block;
	//delete instanceBuffer;
	for (const pair<UINT64, MainChunk*>& chunk : activeChunks)
	{
		delete chunk.second;
	}
	activeChunks.clear();
}

void BlockManager::Update()
{
	//GetSelectedBlock();
	//
	//for (Block* block : blocks)
	//	if (abs(PLAYER->GetGlobalPosition().x - block->GetGlobalPosition().x) < 16
	//		&& abs(PLAYER->GetGlobalPosition().z - block->GetGlobalPosition().z) < 16)
	//	{
	//		block->Update();
	//	}
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
	//instanceBuffer->Set(1);
	//
	//UINT instanceCount = (UINT)instanceDatas.size();
	//block->RenderInstanced(instanceDatas.size());
	//
	//for (Block* block : blocks)
	//	if (abs(PLAYER->GetGlobalPosition().x - block->GetGlobalPosition().x) < 16
	//		&& abs(PLAYER->GetGlobalPosition().z - block->GetGlobalPosition().z) < 16)
	//	{
	//		block->Render();
	////	}
	worldGenerator->Render();
}

void BlockManager::PostRender()
{
}

//void BlockManager::BuildBlock()
//{
//	if (!selectedBlock) return;
//
//	pair<UINT, UINT>quickSlotData = QUICKSLOT->GetSelectedIndexData();
//	
//	if (quickSlotData.first == 0) return;
//
//	ItemData data = DataManager::Get()->GetItemData(quickSlotData.first);
//	
//	if (!data.canBuild) return;
//
//	Block* newBlock = nullptr;
//
//	newBlock = new Block(quickSlotData.first);
//		
//	Ray ray = CAM->ScreenPointToRay(CENTER);
//	RaycastHit hit;
//
//	//if (selectedBlock->IsRayCollision(ray, &hit))
//	//{
//	//	newBlock->SetActive(true);
//	//	newBlock->SetLocalPosition(selectedBlock->GetGlobalPosition() + hit.normal);
//	//	newBlock->UpdateWorld();
//	//
//	//	size_t index = blocks.size();
//	//	newBlock->SetIndex(index);
//	//
//	//	blocks.push_back(newBlock);
//	//
//	//	Vector3 pos = { (float)newBlock->GetGlobalPosition().x, (float)newBlock->GetGlobalPosition().y, (float)newBlock->GetGlobalPosition().z};
//	//
//	//	InstanceData newInstanceData;
//	//	newInstanceData.transform = XMMatrixTranslation(pos.x, pos.y, pos.z);
//	//	newInstanceData.transform = XMMatrixTranspose(newInstanceData.transform);
//	//
//	//	instanceDatas.push_back(newInstanceData);
//	//}
//	//else
//	//{
//	//	delete newBlock;
//	//}
//	//
//	//UpdateInstanceBuffer();
//}
//
//void BlockManager::MiningBlock()
//{
//	//if (!selectedBlock) return;
//	//
//	//size_t index = selectedBlock->GetIndex();
//	//
//	//selectedBlock->Damage();
//	//
//	//if (blocks[index]->IsMining())
//	//{
//	//	instanceDatas.erase(instanceDatas.begin() + index);
//	//
//	//	delete blocks[index]; 
//	//	blocks.erase(blocks.begin() + index);
//	//
//	//	for (size_t i = index; i < blocks.size(); i++)
//	//	{
//	//		blocks[i]->SetIndex(i); 
//	//	}
//	//
//	//	UpdateInstanceBuffer();
//	//
//	//	selectedBlock = nullptr;
//	//}
//}

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



