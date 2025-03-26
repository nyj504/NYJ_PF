#include "Framework.h"
#include "SubChunk.h"

SubChunk::SubChunk(int index, WorldGenerator* worldGenerator) : index(index), worldGenerator(worldGenerator)
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

		if (block->IsOcclusion()) continue;

		Vector3 playerPos = PLAYER->GetGlobalPosition();
		Vector3 blockPos = block->GetGlobalPosition();

		float xDistance = abs(playerPos.x - blockPos.x);
		float yDistance = abs(playerPos.y - blockPos.y);
		float zDistance = abs(playerPos.z - blockPos.z);

		if (xDistance <= 3 && yDistance <= 3 && zDistance <= 3)
		{
			block->Update();
		}

		if (xDistance <= 6 && yDistance <= 6 && zDistance <= 6)
		{
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
	}

	selectedBlock = closestBlock;

	if (selectedBlock)
	{
		isMouseOver = true;
		worldGenerator->SetActiveSubChunk(this);
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

		Vector3 playerPos = PLAYER->GetGlobalPosition();
		Vector3 blockPos = block->GetGlobalPosition();

		float xDistance = abs(playerPos.x - blockPos.x);
		float yDistance = abs(playerPos.y - blockPos.y);
		float zDistance = abs(playerPos.z - blockPos.z);

		if (xDistance <= 6 && yDistance <= 6 && zDistance <= 6)
		{
			block->Render();
		}
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
				Vector3 globalPos = { pos.x + x, pos.y + y, pos.z + z };

				if (worldY > terrainHeight) continue;

				int blockType = 3;

				if (worldY == terrainHeight)
				{
					blockType = 4;
					
					if (x == 8 && z == 8 && !isGenerateTree)
					{
						int treeType = GameMath::Random(0, 2);
						GenerateTree((TreeType)treeType, globalPos);
					}
				}
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
					int randValue = GameMath::Random(1, 101);

					if (worldY < terrainHeight - 5 && worldY >= terrainHeight - 16) 
					{
						if (randValue < 15) blockType = 7;  // ��ź (15% Ȯ��)
						else if (randValue < 5) blockType = 8; // �������� (5% Ȯ��)
						else if (randValue < 2) blockType = 10; // �ݱ��� (2% Ȯ��) 
					}
					else if (worldY < terrainHeight - 16 && worldY >= terrainHeight - 32) 
					{
						if (randValue < 12) blockType = 9;  // ö���� (12% Ȯ��)
						else if (randValue < 5) blockType = 10;  // �ݱ��� (5% Ȯ��) 
						else if (randValue < 4) blockType = 12; // û�ݼ�(4% Ȯ��) 
						else if (randValue < 1) blockType = 11; // ���̾Ƹ�� (1% Ȯ��) 
					}
					else if (worldY < terrainHeight - 32 && worldY >= terrainHeight - 40) 
					{
						if (randValue < 14) blockType = 9;  // ö���� (14% Ȯ��)
						else if (randValue < 6) blockType = 10;  // �ݱ��� (6% Ȯ��) 
						else if (randValue < 5) blockType = 12; // û�ݼ� (5% Ȯ��) 
						else if (randValue < 3) blockType = 13; // ���彺�� (3% Ȯ��)
						else if (randValue < 2) blockType = 11; // ���̾Ƹ�� (2% Ȯ��) 
					}
					else if (worldY < terrainHeight - 40 && worldY >= terrainHeight - 48) 
					{
						if (randValue < 7) blockType = 13;  // ���彺�� (7% Ȯ��)
						else if (randValue < 5) blockType = 12;  // û�ݼ� (5% Ȯ��) 
						else if (randValue < 7) blockType = 10;  // �ݱ��� (7% Ȯ��) 
						else if (randValue < 4) blockType = 11;  // ���̾Ƹ�� (4% Ȯ��) 
					}
				}
				UINT64 blockID = GameMath::GenerateBlockID(globalPos);
				UINT blockIndex = worldGenerator->GetBlockInstanceIndex();
			
				Block* newBlock = new Block(blockType);
				newBlock->SetActive(true);
				newBlock->SetLocalPosition(globalPos);
				newBlock->SetBlockInstanceID(blockIndex);
				newBlock->UpdateWorld();

				blocks[blockID] = newBlock;
			}
		}
	}
}

void SubChunk::GenerateTree(TreeType type,  Vector3 pos)
{
	int treeHeight = GameMath::Random(4, 6);

	int leavesRange = treeHeight - 1;

	int leafType = 0;
	int logType = 0;

	switch (type)
	{
	case SubChunk::CHAM:
		logType = 24;
		leafType = 16;
		break;
	case SubChunk::ACACIA:
		logType = 25;
		leafType = 17;
		break;
	default:
		break;
	}

	for (int y = 0; y < treeHeight; y++)
	{
		Vector3 globalPos = { pos.x, pos.y + y, pos.z };
		UINT64 blockID = GameMath::GenerateBlockID(globalPos);
		UINT blockIndex = worldGenerator->GetBlockInstanceIndex();

		Block* logBlock = new Block(logType);
		logBlock->SetActive(true);
		logBlock->SetLocalPosition(globalPos);
		logBlock->SetBlockInstanceID(blockIndex);
		logBlock->UpdateWorld();

		blocks[blockID] = logBlock;
	}

	for (int x = -leavesRange; x < leavesRange; x++)
	{
		for (int z = -leavesRange; z < leavesRange; z++)
		{
			for (int y = treeHeight; y < treeHeight + leavesRange; y++)
			{
				Vector3 globalPos = { pos.x + x, pos.y + y, pos.z + z };
				UINT64 blockID = GameMath::GenerateBlockID(globalPos);
				UINT blockIndex = worldGenerator->GetBlockInstanceIndex();

				Block* leafBlock = new Block(leafType);
				leafBlock->SetActive(true);
				leafBlock->SetLocalPosition(globalPos);
				leafBlock->SetBlockInstanceID(blockIndex);
				leafBlock->UpdateWorld();

				blocks[blockID] = leafBlock;
			}
		}
	}

	isGenerateTree = true;
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
				} // ��Ŭ���� 

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

				visibleInstanceData.index = block->GetBlockInstanceID();
				visibleInstanceData.isActive = block->IsActive();
					
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
	UINT64 blockID = GameMath::GenerateBlockID(block->GetGlobalPosition());

	unordered_map<UINT64, Block*>::iterator it = blocks.find(blockID);
	
	if (it == blocks.end()) return;

	if (selectedBlock == it->second)
	{
		selectedBlock->SetActive(false);
		selectedBlock = nullptr;
	}	

	BlockManager::Get()->SetSelectedBlock(nullptr);

	FindVisibleBlocks();
}

void SubChunk::BuildBlock(Vector3 pos, int blockType)
{
	UINT64 blockID = GameMath::GenerateBlockID(pos);
	UINT blockIndex = worldGenerator->GetBlockInstanceIndex();	
	unordered_map<UINT64, Block*>::iterator it = blocks.find(blockID);
	
	Block* newBlock = new Block(blockType);
	newBlock->SetLocalPosition(pos);
	newBlock->UpdateWorld();
	newBlock->EnableCollider();
	newBlock->SetActive(true);
	newBlock->SetBlockInstanceID(blockIndex);
	blocks[blockID] = newBlock;

	FindVisibleBlocks();
}
