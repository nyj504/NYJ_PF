#include "Framework.h"
#include "SubChunk.h"

SubChunk::SubChunk(int index) : index(index)
{
}

SubChunk::~SubChunk()
{
	for (auto& pair : blocks)
	{
		delete pair.second;
	}
	blocks.clear();
}

void SubChunk::Update()
{
	isMouseOver = false;

	Ray ray = CAM->ScreenPointToRay(mousePos);

	float minDistance = FLT_MAX;
	RaycastHit hit;
	Block* closestBlock = nullptr;

	Vector3 rayStartPos = (CAM->IsFPSView()) ? CAM->GetGlobalPosition() : PLAYER->GetGlobalPosition();

	for (const pair<UINT64, Block*> pair : blocks)
	{
		Block* block = pair.second;

		block->Update();

		float dist = Vector3::Distance(block->GetGlobalPosition(), rayStartPos);
		float maxDistance = PLAYER->GetPlayerReach(false);

		if (block->GetCollider()->IsRayCollision(ray, &hit))
		{
			if (hit.distance > 0.0f && hit.distance <= maxDistance && hit.distance < minDistance)
			{
				minDistance = hit.distance;
				closestBlock = block;
			}
		}
	}

	selectedBlock = closestBlock;

	if (selectedBlock)
	{
		isMouseOver = true;
	}

	for (const pair<UINT64, Block*> pair : blocks)
	{
		Block* block = pair.second;

		if (block == closestBlock)
			block->GetCollider()->SetColor(1, 0, 0);
		else
			block->GetCollider()->SetColor(0, 1, 0);
	}
	//CheckSelectedBlock();
}

void SubChunk::Render()
{

	for (const pair<UINT64, Block*> pair : blocks)
	{
		Block* block = pair.second;
			block->Render();
	}
	
	//for (Block* visibleBlock : visibleBlocks)
	//{
	//	visibleBlock->Render();
	//}
}

void SubChunk::GenerateTerrain(Vector3 pos, UINT heightMap[CHUNK_WIDTH][CHUNK_DEPTH])
{
	worldPos = pos;
	blocks.clear(); 

	for (UINT x = 0; x < CHUNK_WIDTH; x++)
	{
		for (UINT z = 0; z < CHUNK_DEPTH; z++)
		{
			int minY = pos.y;
			int maxY = minY + SUBCHUNK_HEIGHT;

			int terrainHeight = heightMap[x][z];

			for (UINT y = 0; y < SUBCHUNK_HEIGHT; y++)
			{
				int worldY = minY + y;

				if (worldY > terrainHeight) continue;

				int blockType = 3;  

				if (worldY == terrainHeight)
					blockType = 4; 
				else if (worldY > terrainHeight - 3)
					blockType = 6;  
				else if (worldY < terrainHeight - 6)
				{
					 blockType = 2;
				}
				else if (worldY <= terrainHeight - 47)
				{
					blockType = 1;
				}

				if (blockType == 2 || blockType == 1)
				{
					int randValue = GameMath::Random(1, 100);

					if (worldY < terrainHeight - 5 && worldY >= terrainHeight - 16) 
					{
						if (randValue < 15) blockType = 7;  // ºÆ≈∫ (15% »Æ∑¸)
						else if (randValue < 5) blockType = 8; // ±∏∏Æ±§ºÆ (5% »Æ∑¸)
						else if (randValue < 2) blockType = 10; // ±›±§ºÆ (2% »Æ∑¸) 
					}
					else if (worldY < terrainHeight - 16 && worldY >= terrainHeight - 32) 
					{
						if (randValue < 12) blockType = 9;  // √∂±§ºÆ (12% »Æ∑¸)
						else if (randValue < 5) blockType = 10;  // ±›±§ºÆ (5% »Æ∑¸) 
						else if (randValue < 4) blockType = 12; // √ª±›ºÆ(4% »Æ∑¸) 
						else if (randValue < 1) blockType = 11; // ¥Ÿ¿Ãæ∆∏ÛµÂ (1% »Æ∑¸) 
					}
					else if (worldY < terrainHeight - 32 && worldY >= terrainHeight - 40) 
					{
						if (randValue < 14) blockType = 9;  // √∂±§ºÆ (14% »Æ∑¸)
						else if (randValue < 6) blockType = 10;  // ±›±§ºÆ (6% »Æ∑¸) 
						else if (randValue < 5) blockType = 12; // √ª±›ºÆ (5% »Æ∑¸) 
						else if (randValue < 3) blockType = 13; // ∑πµÂΩ∫≈Ê (3% »Æ∑¸)
						else if (randValue < 2) blockType = 11; // ¥Ÿ¿Ãæ∆∏ÛµÂ (2% »Æ∑¸) 
					}
					else if (worldY < terrainHeight - 40 && worldY >= terrainHeight - 48) 
					{
						if (randValue < 7) blockType = 13;  // ∑πµÂΩ∫≈Ê (7% »Æ∑¸)
						else if (randValue < 5) blockType = 12;  // √ª±›ºÆ (5% »Æ∑¸) 
						else if (randValue < 7) blockType = 10;  // ±›±§ºÆ (7% »Æ∑¸) 
						else if (randValue < 4) blockType = 11;  // ¥Ÿ¿Ãæ∆∏ÛµÂ (4% »Æ∑¸) 
					}
				}
				Vector3 globalPos = { pos.x + x, pos.y + y, pos.z + z };
				UINT64 blockID = GameMath::GenerateBlockID(globalPos);
			
				Block* newBlock = new Block(blockType);
				newBlock->SetActive(true);
				newBlock->SetLocalPosition(globalPos);
				newBlock->SetBlockID(blockID);
				newBlock->SetParentIndex(parentIndex);
				newBlock->UpdateWorld();

				blocks[blockID] = newBlock;
			}
		}
	}
}

Block* SubChunk::GetBlock(Vector3 globalPos)
{
	UINT64 blockID = GameMath::GenerateBlockID(globalPos);

	if (blocks.count(blockID) > 0 && blocks[blockID]->IsActive())
	{
		return blocks[blockID];
	}
}

void SubChunk::FindVisibleBlocks()
{
	visibleBlocks.clear();
	visibleBlocks.reserve(CHUNK_WIDTH * SUBCHUNK_HEIGHT * CHUNK_DEPTH);
	visibleSingleInstanceDatas.clear();
	visibleMultiInstanceDatas.clear();

	Vector3 playerPos = PLAYER->GetGlobalPosition();

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_DEPTH; z++)
		{
			for (int y = 0; y < SUBCHUNK_HEIGHT; y++)
			{
				Vector3 blockPos = { worldPos.x + x, worldPos.y + y, worldPos.z + z };
				Block* block = GetBlock(blockPos);

				if (!block) continue;

				Vector3 blockWorldPos = block->GetGlobalPosition();

				if (GetBlock({ blockWorldPos.x + 1, blockWorldPos.y, blockWorldPos.z }) &&
					GetBlock({ blockWorldPos.x - 1, blockWorldPos.y, blockWorldPos.z }) &&
					GetBlock({ blockWorldPos.x, blockWorldPos.y + 1, blockWorldPos.z }) &&
					GetBlock({ blockWorldPos.x, blockWorldPos.y - 1, blockWorldPos.z }) &&
					GetBlock({ blockWorldPos.x, blockWorldPos.y, blockWorldPos.z + 1 }) &&
					GetBlock({ blockWorldPos.x, blockWorldPos.y, blockWorldPos.z - 1 }))
				{
					block->SetOcclusion(true);
					continue;
				} // ø¿≈¨∑Á¡Ø 

				if (blockWorldPos.y <= playerPos.y - 7)
				{
					continue;
				}

				InstanceData visibleInstanceData;
				UVInfo uvInfo = block->GetUVInfo();

				block->SetOcclusion(false);
				visibleBlocks.push_back(block);

				visibleInstanceData.transform = XMMatrixTranslation(blockWorldPos.x, blockWorldPos.y, blockWorldPos.z);
				visibleInstanceData.transform = XMMatrixTranspose(visibleInstanceData.transform);

				visibleInstanceData.curFrame = uvInfo.uvStart;
				visibleInstanceData.maxFrame = uvInfo.uvEnd;

				visibleInstanceData.blockID = block->GetBlockID();

				if (block->IsNormal())
				{
					visibleSingleInstanceDatas.push_back(visibleInstanceData);
				}
				else
				{
					visibleMultiInstanceDatas.push_back(visibleInstanceData);
				}
			}
		}
	}
}

void SubChunk::CheckVisibleBlocks()
{
	FindVisibleBlocks();
}

void SubChunk::ActiveCollider()
{
	for (const pair<UINT64, Block*> pair : blocks)
	{
		Block* block = pair.second;
		block->EnableCollider();  
	}

	hasCollider = true;
}

void SubChunk::MiningBlock(Block* block)
{
	UINT64 blockID = block->GetBlockID();

	unordered_map<UINT64, Block*>::iterator it = blocks.find(blockID);
	
	if (it == blocks.end()) return;

	if (selectedBlock == it->second)
	{
		delete it->second;
		blocks.erase(it);
		selectedBlock = nullptr;
	}	

	BlockManager::Get()->SetSelectedBlock(nullptr);
}

void SubChunk::BuildBlock(Vector3 pos, int blockType)
{
	UINT64 blockID = GameMath::GenerateBlockID(pos);
	
	Block* newBlock = new Block(blockType);
	newBlock->EnableCollider();
	newBlock->SetLocalPosition(pos);
	newBlock->SetParentIndex(parentIndex);
	newBlock->SetActive(true);
	newBlock->SetBlockID(blockID);
	newBlock->UpdateWorld();

	blocks[blockID] = newBlock;
}

void SubChunk::CheckSelectedBlock()
{
	isMouseOver = false;

	Ray ray = CAM->ScreenPointToRay(mousePos);

	float minDistance = FLT_MAX;
	RaycastHit hit;
	Block* closestBlock = nullptr;

	Vector3 rayStartPos = (CAM->IsFPSView()) ? CAM->GetGlobalPosition() : PLAYER->GetGlobalPosition();

	for (Block* visibleBlock : visibleBlocks)
	{
		float dist = Vector3::Distance(visibleBlock->GetGlobalPosition(), rayStartPos);
		float maxDistance = PLAYER->GetPlayerReach(false);

		if (visibleBlock->GetCollider()->IsRayCollision(ray, &hit) && hit.distance <= maxDistance)
		{
			if (hit.distance < minDistance)
			{
				minDistance = hit.distance;
				closestBlock = visibleBlock;
				isMouseOver = true;
			}
		}
	}

	selectedBlock = closestBlock;

	for (pair<const UINT64, Block*> pair : blocks)
	{
		if (pair.second == closestBlock)
			pair.second->GetCollider()->SetColor(1, 0, 0);
		else
			pair.second->GetCollider()->SetColor(0, 1, 0);
	}
}

