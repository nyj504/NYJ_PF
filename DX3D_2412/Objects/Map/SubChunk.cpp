#include "Framework.h"
#include "SubChunk.h"

SubChunk::SubChunk(int index) : index(index)
{
}

SubChunk::~SubChunk()
{
}

void SubChunk::Update()
{
	for (pair<const UINT64, Block>& pair : blocks)
	{
		Block& block = pair.second;
		block.Update();  
	}
}

void SubChunk::Render()
{
	for (pair<const UINT64, Block>& pair : blocks)
	{
		Block& block = pair.second;
		block.Render();  // ºí·Ï ·»´õ¸µ
	}
}

void SubChunk::GenerateTerrain(Vector3 pos, UINT heightMap[CHUNK_WIDTH + 1][CHUNK_DEPTH + 1])
{
	worldPos = pos;
	blocks.clear(); 

	for (UINT x = 0; x < CHUNK_WIDTH; x++)
	{
		for (UINT z = 0; z < SUBCHUNK_HEIGHT; z++)
		{
			int minY = pos.y;
			int maxY = minY + SUBCHUNK_HEIGHT;

			int terrainHeight = heightMap[x][z];

			for (UINT y = 0; y < CHUNK_DEPTH; y++)
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

					if (worldY < terrainHeight - 5 && worldY >= terrainHeight - 16) // Y=33~48 (¾èÀº ÁöÇÏÃş)
					{
						if (randValue < 15) blockType = 7;  // ¼®Åº (15% È®·ü)
						else if (randValue < 5) blockType = 8; // ±¸¸®±¤¼® (5% È®·ü)
						else if (randValue < 2) blockType = 10; // ±İ±¤¼® (2% È®·ü) 
					}
					else if (worldY < terrainHeight - 16 && worldY >= terrainHeight - 32) // Y=17~32 (Áß°£ ÁöÇÏÃş)
					{
						if (randValue < 12) blockType = 9;  // Ã¶±¤¼® (12% È®·ü)
						else if (randValue < 5) blockType = 10;  // ±İ±¤¼® (5% È®·ü) 
						else if (randValue < 4) blockType = 12; // Ã»±İ¼®(¶óÇÇ½º ¶óÁÙ¸®) (4% È®·ü) 
						else if (randValue < 1) blockType = 11; // ´ÙÀÌ¾Æ¸óµå (1% È®·ü) 
					}
					else if (worldY < terrainHeight - 32 && worldY >= terrainHeight - 40) // Y=9~16 (±íÀº ÁöÇÏÃş)
					{
						if (randValue < 14) blockType = 9;  // Ã¶±¤¼® (14% È®·ü)
						else if (randValue < 6) blockType = 10;  // ±İ±¤¼® (6% È®·ü) 
						else if (randValue < 5) blockType = 12; // Ã»±İ¼® (5% È®·ü) 
						else if (randValue < 3) blockType = 13; // ·¹µå½ºÅæ (3% È®·ü)
						else if (randValue < 2) blockType = 11; // ´ÙÀÌ¾Æ¸óµå (2% È®·ü) 
					}
					else if (worldY < terrainHeight - 40 && worldY >= terrainHeight - 48) // Y=1~8 (½ÉÃş ÁöÇÏÃş)
					{
						if (randValue < 7) blockType = 13;  // ·¹µå½ºÅæ (7% È®·ü)
						else if (randValue < 5) blockType = 12;  // Ã»±İ¼® (5% È®·ü) 
						else if (randValue < 7) blockType = 10;  // ±İ±¤¼® (7% È®·ü) 
						else if (randValue < 4) blockType = 11;  // ´ÙÀÌ¾Æ¸óµå (4% È®·ü) 
					}
				}
				Vector3 globalPos = { pos.x + x, pos.y + y, pos.z + z };
				UINT64 blockID = GameMath::GenerateBlockID(globalPos);
			
				pair<unordered_map<UINT64, Block>::iterator, bool> result = blocks.emplace(blockID, Block(blockType));

				if (result.second) 
				{
					Block& newBlock = result.first->second;
					newBlock.SetParent(this);
					newBlock.SetLocalPosition(globalPos);
					newBlock.SetBlockID(blockID);
					newBlock.SetParentIndex(parentIndex);
					newBlock.UpdateWorld();
				}
			}
		}
	}
}

Block* SubChunk::GetBlock(Vector3 globalPos)
{
	UINT64 blockID = GameMath::GenerateBlockID(globalPos);

	if (blocks.count(blockID) > 0) 
	{
		return &blocks[blockID];
	}
}

void SubChunk::FindVisibleBlocks()
{
	visibleBlocks.clear();
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

				if (!block || block->IsMining()) continue;

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
				} // ¿ÀÅ¬·çÁ¯ 

				if (blockWorldPos.y <= playerPos.y - 7)
				{
					continue;
				}

				InstanceData visibleInstanceData;
				UVInfo uvInfo = block->GetUVInfo();

				block->SetOcclusion(false);

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
	for (auto& pair : blocks)
	{
		Block& block = pair.second;
		block.EnableCollider();  
	}

	hasCollider = true;
}

void SubChunk::MiningBlock(Block* block)
{
	UINT64 blockID = block->GetBlockID();

	auto it = blocks.find(blockID);
	blocks.erase(blockID);
}

void SubChunk::BuildBlock(Vector3 pos, int blockType)
{
	UINT64 blockID = GameMath::GenerateBlockID(pos);
	pair<unordered_map<UINT64, Block>::iterator, bool> result = blocks.emplace(blockID, Block(blockType));

	if (result.second)
	{
		Block& newBlock = result.first->second;
		newBlock.SetParent(this);
		newBlock.EnableCollider();
		newBlock.SetLocalPosition(pos);
		newBlock.SetBlockID(blockID);
		newBlock.UpdateWorld();
	}
}

void SubChunk::CheckSelectedBlock()
{
	Ray ray = CAM->ScreenPointToRay(mousePos);

	float minDistance = FLT_MAX;
	RaycastHit hit;
	Block* closestBlock = nullptr;

	Vector3 rayStartPos = (CAM->IsFPSView()) ? CAM->GetGlobalPosition() : PLAYER->GetGlobalPosition();

	for (pair<const UINT64, Block>& pair : blocks)
	{
		Block& block = pair.second;
		if (block.IsOcclusion()) continue;
		if (!block.IsActive()) continue;

		float dist = Vector3::Distance(block.GetGlobalPosition(), rayStartPos);
		float maxDistance = PLAYER->GetPlayerReach(block.GetBlockType());

		if (block.GetCollider()->IsRayCollision(ray, &hit) && hit.distance < maxDistance)
		{
			if (hit.distance < minDistance)
			{
				minDistance = hit.distance;
				closestBlock = &block;
				selectedBlock = closestBlock;
			}
		}
	}

	for (pair<const UINT64, Block>& pair : blocks)
	{
		if (&pair.second == closestBlock)
			pair.second.GetCollider()->SetColor(1, 0, 0);
		else
			pair.second.GetCollider()->SetColor(0, 1, 0);
	}
}
