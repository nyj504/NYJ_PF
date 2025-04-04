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

	Vector3 playerPos = PLAYER->GetLocalPosition();
	float maxDistance = PLAYER->GetPlayerReach(false);

	for (const pair<UINT64, Block*> pair : blocks)
	{
		Block* block = pair.second;

		if (block->IsOcclusion()) continue;
		if (!block->IsActive()) continue;

		Vector3 blockPos = block->GetLocalPosition();

		float xDistance = abs(playerPos.x - blockPos.x);
		float yDistance = abs(playerPos.y - blockPos.y);
		float zDistance = abs(playerPos.z - blockPos.z);

		if (xDistance <= 6 && yDistance <= 6 && zDistance <= 6)
		{
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
		BlockManager::Get()->SetSelectedBlock(selectedBlock);
	}

	for (const pair<UINT64, Block*> pair : blocks)
	{
		Block* block = pair.second;

		if (block == closestBlock)
			block->GetCollider()->SetColor(1, 0, 0);
		else
			block->GetCollider()->SetColor(0, 1, 0);
	}
	CheckPlayerCollision();

	CheckMonsterCollision();
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
}

void SubChunk::GenerateTerrain(Vector3 pos, UINT heightMap[CHUNK_WIDTH][CHUNK_DEPTH])
{
	worldPos = pos;
	blocks.clear();

	vector<Vector3> treePositions;

	if (index == 0)
	{
		int treeCount = GameMath::Random(0, 4);

		for (int i = 0; i < treeCount; i++)
		{
			int treeX = GameMath::Random(2, CHUNK_WIDTH - 2);
			int treeZ = GameMath::Random(2, CHUNK_DEPTH - 2);
			int treeY = heightMap[treeX][treeZ];

			treePositions.push_back({ (float)treeX, (float)treeY, (float)treeZ });
		}
	}

	for (UINT x = 0; x < CHUNK_WIDTH; x++)
	{
		for (UINT z = 0; z < CHUNK_DEPTH; z++)
		{
			int minY = pos.y;
			int maxY = minY + SUBCHUNK_HEIGHT;

			int terrainHeight = heightMap[x][z];

			for (const auto& treePos : treePositions)
			{
				if (treePos.x == x && treePos.y == terrainHeight && treePos.z == z)
				{
					int treeType = GameMath::Random(0, 2);
					Vector3 globalTreePos = { treePos.x + pos.x, treePos.y, treePos.z + pos.z };
					GenerateTree((TreeType)treeType, globalTreePos);
				}
			}

			for (UINT y = 0; y < SUBCHUNK_HEIGHT; y++)
			{
				int worldY = minY + y;
				Vector3 globalPos = { pos.x + x, pos.y + y, pos.z + z };

				if (worldY > terrainHeight) continue;

				int blockType = 3;

				if (worldY == terrainHeight)
				{
					blockType = 4;
				}
				else if (worldY > terrainHeight - 3) blockType = 6;
				else if (worldY < terrainHeight - 6) blockType = 2;
				else if (worldY <= terrainHeight - 47) blockType = 1;

				if (blockType == 2 || blockType == 1)
				{
					int randValue = GameMath::Random(1, 101);

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
	int treeHeight = 0;
	int leavesRange = 3;
	int leafType = 0;
	int logType = 0;

	switch (type)
	{
	case SubChunk::CHAM:
		treeHeight = GameMath::Random(4, 6);
		logType = 24;
		leafType = 16;

		for (int y = -1; y <= 2; y++) 
		{
			int leafY = pos.y + treeHeight + y;
			int radius = 2 - abs(y); 

			for (int x = -radius; x <= radius; x++)
			{
				for (int z = -radius; z <= radius; z++)
				{
					if (x == 0 && z == 0 && y <= 0) continue;
					if (abs(x) == radius && abs(z) == radius)
					{
						if (GameMath::Random(0, 100) < 50) continue;
					}

					Vector3 globalPos = { pos.x + x, (float)leafY, pos.z + z };
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
		break;
	case SubChunk::ACACIA:
		treeHeight = GameMath::Random(5, 8);
		logType = 25;
		leafType = 17;

		for (int y = 0; y < leavesRange; y++)
		{
			int leafY = treeHeight + y;
			int offset = leavesRange - y;

			for (int x = -offset; x <= offset; x++)
			{
				for (int z = -offset; z <= offset; z++)
				{
					Vector3 globalPos = { pos.x + x, pos.y + leafY, pos.z + z };

					UINT64 blockID = GameMath::GenerateBlockID(globalPos);
					UINT blockIndex = worldGenerator->GetBlockInstanceIndex();

					if (x == 0 && z == 0) // ≥™π´ ∏ˆ≈Î
					{
						Block* logBlock = new Block(logType);
						logBlock->SetActive(true);
						logBlock->SetLocalPosition(globalPos);
						logBlock->SetBlockInstanceID(blockIndex);
						logBlock->UpdateWorld();

						blocks[blockID] = logBlock;
					}
					else
					{
						if (y % 2 != 0) continue;

						Block* leafBlock = new Block(leafType);
						leafBlock->SetActive(true);
						leafBlock->SetLocalPosition(globalPos);
						leafBlock->SetBlockInstanceID(blockIndex);
						leafBlock->UpdateWorld();

						blocks[blockID] = leafBlock;
					}
				}
			}
		}
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
}

void SubChunk::CheckPlayerCollision()
{
	Ray ray(PLAYER->GetLocalPosition(), Vector3::Down());
	RaycastHit hit;

	float maxHeight = 0.0f;
	float epsilonY = 0.1f;
	Vector3 playerPos = PLAYER->GetLocalPosition();
	Vector3 maxPlayerPosition = PLAYER->GetLocalPosition() + PLAYER->GetCollider()->HalfSize();
	Vector3 minPlayerPosition = PLAYER->GetLocalPosition() - PLAYER->GetCollider()->HalfSize();
	Vector3 overlap;

	for (const pair<UINT64, Block*> pair : blocks)
	{
		Block* block = pair.second;

		Vector3 blockPos = block->GetLocalPosition();

		float xDistance = abs(playerPos.x - blockPos.x);
		float yDistance = abs(playerPos.y - blockPos.y);
		float zDistance = abs(playerPos.z - blockPos.z);

		if (block->IsOcclusion() || !block->IsActive())
			continue;

		if (xDistance > 2 && yDistance > 2 && zDistance > 2)
			continue;

		if (block->GetCollider()->IsRayCollision(ray, &hit))
		{
			if (hit.point.y > maxHeight)
				maxHeight = hit.point.y;
		}	
	}

	if (maxHeight >= minPlayerPosition.y)
	{
		PLAYER->Translate(0, maxHeight - minPlayerPosition.y, 0);
		PLAYER->SetLand();
	}
	else if (maxHeight - minPlayerPosition.y < 0.1f)
	{
		PLAYER->SetFall();
	}

	for (const pair<UINT64, Block*> pair : blocks)
	{
		Block* block = pair.second;

		if (block->IsOcclusion() || !block->IsActive())
			continue;

		Vector3 blockPos = block->GetLocalPosition();

		float xDistance = abs(playerPos.x - blockPos.x);
		float yDistance = abs(playerPos.y - blockPos.y);
		float zDistance = abs(playerPos.z - blockPos.z);

		if (xDistance <= 2 && yDistance <= 2 && zDistance <= 2)
		{
			Vector3 blockPosition = block->GetLocalPosition();
			Vector3 maxBoxPosition = blockPosition + block->GetCollider()->HalfSize();
			Vector3 minBoxPosition = blockPosition - block->GetCollider()->HalfSize();

			if (block->GetCollider()->IsBoxCollision(PLAYER->GetCollider(), &overlap))
			{
				if (minPlayerPosition.y < maxBoxPosition.y - epsilonY &&
					maxPlayerPosition.y > minBoxPosition.y + epsilonY)
				{
					if (abs(overlap.x) < abs(overlap.z))
					{
						if (playerPos.x < blockPos.x)
							PLAYER->Translate(-overlap.x, 0, 0);
						else
							PLAYER->Translate(overlap.x, 0, 0);
					}
					else
					{
						if (playerPos.z < blockPos.z)
							PLAYER->Translate(0, 0, -overlap.z);
						else
							PLAYER->Translate(0, 0, overlap.z);
					}
				}
			}
		}
	}
}

void SubChunk::CheckMonsterCollision()
{
	Monster* monster = MonsterManager::Get()->GetMonsters();

	if (!monster) return;

	Ray ray(monster->GetLocalPosition(), Vector3::Down());
	RaycastHit hit;

	float maxHeight = 0.0f;
	float epsilonY = 0.1f;

	Vector3 monsterPos = monster->GetLocalPosition();
	Vector3 maxMonsterPosition = monsterPos + monster->GetCollider()->HalfSize();
	Vector3 minMonsterPosition = monsterPos - monster->GetCollider()->HalfSize();
	Vector3 overlap;

	for (const pair<UINT64, Block*> pair : blocks)
	{
		Block* block = pair.second;

		Vector3 blockPos = block->GetLocalPosition();

		float xDistance = abs(monsterPos.x - blockPos.x);
		float yDistance = abs(monsterPos.y - blockPos.y);
		float zDistance = abs(monsterPos.z - blockPos.z);

		if (block->IsOcclusion() || !block->IsActive())
			continue;

		if (xDistance > 2 && yDistance > 2 && zDistance > 2)
			continue;

		if (block->GetCollider()->IsRayCollision(ray, &hit))
		{
			if (hit.point.y > maxHeight)
				maxHeight = hit.point.y;
		}
	}

	if (maxHeight >= minMonsterPosition.y)
	{
		monster->Translate(0, maxHeight - minMonsterPosition.y, 0);
		monster->SetLand();
	}
	else if (maxHeight - minMonsterPosition.y < 0.1f)
	{
		monster->SetFall();
	}

	for (const pair<UINT64, Block*> pair : blocks)
	{
		Block* block = pair.second;

		if (block->IsOcclusion() || !block->IsActive())
			continue;

		Vector3 blockPos = block->GetLocalPosition();

		float xDistance = abs(monsterPos.x - blockPos.x);
		float yDistance = abs(monsterPos.y - blockPos.y);
		float zDistance = abs(monsterPos.z - blockPos.z);

		if (xDistance <= 2 && yDistance <= 2 && zDistance <= 2)
		{
			Vector3 blockPosition = block->GetLocalPosition();
			Vector3 maxBoxPosition = blockPosition + block->GetCollider()->HalfSize();
			Vector3 minBoxPosition = blockPosition - block->GetCollider()->HalfSize();

			if (block->GetCollider()->IsBoxCollision(monster->GetCollider(), &overlap))
			{
				if (minMonsterPosition.y < maxBoxPosition.y - epsilonY &&
					maxMonsterPosition.y > minBoxPosition.y + epsilonY)
				{
					if (abs(overlap.x) < abs(overlap.z))
					{
						if (monsterPos.x < blockPos.x)
							monster->Translate(-overlap.x, 0, 0);
						else
							monster->Translate(overlap.x, 0, 0);
					}
					else
					{
						if (monsterPos.z < blockPos.z)
							monster->Translate(0, 0, -overlap.z);
						else
							monster->Translate(0, 0, overlap.z);
					}
				}
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

void SubChunk::ActiveCollider()
{
	for (const pair<UINT64, Block*> pair : blocks)
	{
		Block* block = pair.second;
		block->EnableCollider();  
	}

	hasCollider = true;
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
