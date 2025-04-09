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
	Vector3 playerPos = PLAYER->GetLocalPosition();
	Vector3 minPlayerPosition = PLAYER->GetLocalPosition() - PLAYER->GetCollider()->HalfSize();

	int activeIndex = abs((int)floor(playerPos.y / SUBCHUNK_HEIGHT));

	CheckMousePosBlock();
	
	if (activeIndex == index)
		CheckPlayerCollision();
	
	if (activeIndex == index)
		CheckMonsterCollision();
}

void SubChunk::Render()
{
	for (Block* block : interactableBlocks)
	{
		Vector3 playerPos = PLAYER->GetGlobalPosition();
		Vector3 blockPos = block->GetGlobalPosition();

		float distance = Vector3::Distance(playerPos, blockPos);

		if (distance <= INTERACTABLE_DISTANCE)
		{
			block->Render();
		}
	}
}

void SubChunk::GenerateTerrain(Vector3 pos, UINT heightMap[CHUNK_WIDTH][CHUNK_DEPTH])
{
	worldPos = pos;
	blocks.clear();

	depthLevel = GetDepthLevelFromWorldY(pos.y);

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

				int blockType = 3; // 모래

				if (worldY == terrainHeight) blockType = 4; // 풀
				else if (worldY > terrainHeight - 3) blockType = 6; // 흙
				else if (worldY < terrainHeight - 6) blockType = 2; // 돌
				else if (worldY <= terrainHeight - 47) blockType = 1; //심층암

				if (blockType == 2 || blockType == 1)
				{
					int randValue = GameMath::Random(1, 101);

					switch (depthLevel)
					{
					case DepthLevel::GROUND:
						break;
					case DepthLevel::SHALLOWS:
					{
						if (randValue <= 15) blockType = 7;  // 석탄
						else if (randValue <= 20) blockType = 8; // 구리
						else if (randValue <= 22) blockType = 10; // 금
					}
						break;
					case DepthLevel::MIDDEPTH:
					{
						if (randValue <= 14) blockType = 9;  // 철
						else if (randValue <= 20) blockType = 10; // 금
						else if (randValue <= 25) blockType = 12; // 청금석
						else if (randValue <= 28) blockType = 13; // 레드스톤
						else if (randValue <= 30) blockType = 11; // 다이아
					}
						break;
					case DepthLevel::ABYSS:
					{
						if (randValue <= 7)  blockType = 13; // 레드스톤
						else if (randValue <= 12) blockType = 12; // 청금석
						else if (randValue <= 19) blockType = 10; // 금
						else if (randValue <= 23) blockType = 11; // 다이아
					}
						break;
					default:
						break;
					}
				}
				UINT64 blockID = GameMath::GenerateBlockID(globalPos);
				UINT blockIndex = worldGenerator->GetBlockInstanceIndex();
			
				Block* newBlock = new Block(blockType);
				newBlock->SetActive(true);
				newBlock->SetParentIndex(parentIndex);
				newBlock->SetLocalPosition(globalPos);
				newBlock->SetBlockInstanceID(blockIndex);
				newBlock->SetBlockID(blockID);
				newBlock->UpdateWorld();

				blocks[blockID] = newBlock;
			}
		}
	}
}

DepthLevel SubChunk::GetDepthLevelFromWorldY(int y)
{
	if (y >= 0) return DepthLevel::GROUND;
	else if (y >= -15) return DepthLevel::SHALLOWS;
	else if (y >= -31) return DepthLevel::MIDDEPTH;
	else if (y >= -39) return DepthLevel::ABYSS;
	else return DepthLevel::ABYSS;
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
					leafBlock->SetParentIndex(parentIndex);
					leafBlock->SetLocalPosition(globalPos);
					leafBlock->SetBlockInstanceID(blockIndex);
					leafBlock->SetBlockID(blockID);
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

					if (x == 0 && z == 0) // 나무 몸통
					{
						Block* logBlock = new Block(logType);
						logBlock->SetActive(true);
						logBlock->SetParentIndex(parentIndex);
						logBlock->SetLocalPosition(globalPos);
						logBlock->SetBlockInstanceID(blockIndex);
						logBlock->SetBlockID(blockID);
						logBlock->UpdateWorld();

						blocks[blockID] = logBlock;
					}
					else
					{
						if (y % 2 != 0) continue;

						Block* leafBlock = new Block(leafType);
						leafBlock->SetActive(true);
						leafBlock->SetParentIndex(parentIndex);
						leafBlock->SetLocalPosition(globalPos);
						leafBlock->SetBlockInstanceID(blockIndex);
						leafBlock->SetBlockID(blockID);
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
		logBlock->SetParentIndex(parentIndex);
		logBlock->SetLocalPosition(globalPos);
		logBlock->SetBlockInstanceID(blockIndex);
		logBlock->SetBlockID(blockID);
		logBlock->UpdateWorld();

		blocks[blockID] = logBlock;
	}
}

void SubChunk::CheckMousePosBlock()
{
	Vector3 playerPos = PLAYER->GetLocalPosition();
	Ray ray = CAM->ScreenPointToRay(mousePos);

	float minDistance = FLT_MAX;
	RaycastHit hit;
	Block* closestBlock = nullptr;

	for (Block* block : interactableBlocks)
	{
		Vector3 blockPos = block->GetLocalPosition();

		if (block->GetCollider()->IsRayCollision(ray, &hit))
		{
			if (hit.distance <= INTERACTABLE_DISTANCE && hit.distance < minDistance)
			{
				minDistance = hit.distance;
				closestBlock = block;
			}
		}
	}

	Block* prevSelected = BlockManager::Get()->GetSelectedBlock();

	if (closestBlock)
	{
		if (prevSelected)
		{
			float prevSelectedDistance = Vector3::Distance(playerPos, prevSelected->GetLocalPosition());

			if (minDistance < prevSelectedDistance)
			{
				worldGenerator->SetActiveSubChunk(this);
				BlockManager::Get()->SetSelectedBlock(closestBlock);
			}
			else
			{
				BlockManager::Get()->SetSelectedBlock(prevSelected);
			}
		}
		else
		{
			worldGenerator->SetActiveSubChunk(this);
			BlockManager::Get()->SetSelectedBlock(closestBlock);
		}
	}
	else
	{
		if (!prevSelected)
		{
			BlockManager::Get()->SetSelectedBlock(nullptr);
		}
	}

	for (Block* block : interactableBlocks)
	{
		block->GetCollider()->SetColor(
			(block == closestBlock) ? 1 : 0,
			(block == closestBlock) ? 0 : 1,0);
	}
}

void SubChunk::CheckPlayerCollision()
{
	Ray ray(PLAYER->GetLocalPosition(), Vector3::Down());
	RaycastHit hit;

	float subchunkMaxY = worldPos.y + SUBCHUNK_HEIGHT - 1;

	float maxHeight = -64.0f;

	float epsilonY = 0.1f;

	Vector3 playerPos = PLAYER->GetLocalPosition();
	Vector3 maxPlayerPosition = PLAYER->GetLocalPosition() + PLAYER->GetCollider()->HalfSize();
	Vector3 minPlayerPosition = PLAYER->GetLocalPosition() - PLAYER->GetCollider()->HalfSize();
	Vector3 overlap;

	for (Block* block : interactableBlocks)
	{
		Vector3 blockPos = block->GetLocalPosition();
	
		float distanceX = Vector3::Distance(playerPos.x, blockPos.x);
		float distanceY = Vector3::Distance(playerPos.y, blockPos.y);
		float distanceZ = Vector3::Distance(playerPos.z, blockPos.z);
	
		if (distanceX > 2 && distanceZ > 2 && distanceY > 4)
			continue;
	
		if (block->GetCollider()->IsRayCollision(ray, &hit))
		{
			if (hit.point.y > maxHeight)
			{
				maxHeight = hit.point.y;
				steppedBlock = block;
			}
		}	
	
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

	
	if (!steppedBlock)
	{
		PLAYER->SetFall();
		return;
	}
	
	if (maxHeight >= minPlayerPosition.y)
	{
		Vector3 playerVelocity = PLAYER->GetPlayerVelocity();
	
		PLAYER->Translate(0, maxHeight - minPlayerPosition.y, 0);
		PLAYER->SetLand();
	
		if (playerVelocity.x >= 0.1f || playerVelocity.z >= 0.1f)
		{
			string stepSound;
			if (steppedBlock)
				stepSound = "step_" + steppedBlock->GetItemData().soundType;
	
			if (!Audio::Get()->IsPlaySound(stepSound))
			{
				Audio::Get()->Play(stepSound);
			}
		}
	}
	
}

void SubChunk::CheckMonsterCollision()
{
	Character* monster = MonsterManager::Get()->GetMonsters();

	if (!monster) return;

	Ray ray(monster->GetLocalPosition(), Vector3::Down());
	RaycastHit hit;

	float maxHeight = 0.0f;
	float epsilonY = 0.1f;

	Vector3 monsterPos = monster->GetLocalPosition();
	Vector3 maxMonsterPosition = monsterPos + monster->GetCollider()->HalfSize();
	Vector3 minMonsterPosition = monsterPos - monster->GetCollider()->HalfSize();
	Vector3 overlap;

	for (Block* block : interactableBlocks)
	{
		Vector3 blockPos = block->GetLocalPosition();

		float distance = Vector3::Distance(monsterPos, blockPos);

		if (distance > 2)
			continue;

		if (block->GetCollider()->IsRayCollision(ray, &hit))
		{
			if (hit.point.y > maxHeight)
				maxHeight = hit.point.y;
		}
		if (distance <= 2)
		{
			Vector3 maxBoxPosition = blockPos + block->GetCollider()->HalfSize();
			Vector3 minBoxPosition = blockPos - block->GetCollider()->HalfSize();

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

	if (maxHeight >= minMonsterPosition.y)
	{
		monster->Translate(0, maxHeight - minMonsterPosition.y, 0);
		monster->SetLand();
	}
	else if (maxHeight - minMonsterPosition.y < 0.1f)
	{
		monster->SetFall();
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
	interactableBlocks.clear();

	Vector3 playerPos = PLAYER->GetGlobalPosition();

	for (const pair<UINT64, Block*> pair : blocks)
	{
		Block* block = pair.second;

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
		} // 오클루젼 

		if (blockWorldPos.y <= playerPos.y - 7)
		{
			continue;
		}

		InstanceData visibleInstanceData;
		UVInfo uvInfo = block->GetUVInfo(); 

		block->SetOcclusion(false); // 블록이 한 방위라도 노출되면 인스턴스 데이터 추가

		visibleInstanceData.transform = XMMatrixTranslation(blockWorldPos.x, blockWorldPos.y, blockWorldPos.z);
		visibleInstanceData.transform = XMMatrixTranspose(visibleInstanceData.transform);

		visibleInstanceData.curFrame = uvInfo.uvStart; 
		visibleInstanceData.maxFrame = uvInfo.uvEnd;

		visibleInstanceData.index = block->GetBlockInstanceID(); //블록 고유의 인스턴스 아이디 
		visibleInstanceData.isActive = block->IsActive();
					
		if (block->IsNormal()) // 모든 면이 동일한 블럭 
			visibleSingleInstanceDatas.push_back(visibleInstanceData);
		else // 각기 다른 면을 가진 블럭 
			visibleMultiInstanceDatas.push_back(visibleInstanceData);

		interactableBlocks.push_back(block);
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

void SubChunk::Test()
{
	for (const pair<UINT64, Block*> pair : blocks)
	{
		Block* block = pair.second;
		
		InstanceData visibleInstanceData;
		UVInfo uvInfo = block->GetUVInfo();

		Vector3 blockWorldPos = block->GetGlobalPosition();

		visibleInstanceData.transform = XMMatrixTranslation(blockWorldPos.x, blockWorldPos.y, blockWorldPos.z);
		visibleInstanceData.transform = XMMatrixTranspose(visibleInstanceData.transform);

		visibleInstanceData.curFrame = uvInfo.uvStart;
		visibleInstanceData.maxFrame = uvInfo.uvEnd;

		visibleInstanceData.index = block->GetBlockInstanceID();

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

void SubChunk::BuildBlock(Vector3 pos, int blockType)
{
	UINT64 blockID = GameMath::GenerateBlockID(pos);
	UINT blockIndex = worldGenerator->GetBlockInstanceIndex();	
	unordered_map<UINT64, Block*>::iterator it = blocks.find(blockID);
	
	Block* newBlock = new Block(blockType);
	newBlock->SetParentIndex(parentIndex);
	newBlock->SetLocalPosition(pos);
	newBlock->UpdateWorld();
	newBlock->EnableCollider();
	newBlock->SetActive(true);
	newBlock->SetOcclusion(false);
	newBlock->SetBlockInstanceID(blockIndex);
	newBlock->SetBlockID(blockID);
	blocks[blockID] = newBlock;

	FindVisibleBlocks();
}

void SubChunk::Save()
{
	string path = "Resources/Transforms/Map" + to_string(parentIndex) + to_string(index) + ".srt";
	BinaryWriter* writer = new BinaryWriter(path);

	int count = blocks.size();
	writer->Data<int>(count); 

	for (const pair<UINT64, Block*> pair : blocks)
	{
		Block* block = pair.second;

		writer->Data<UINT>(block->GetItemData().key);
		writer->Data<bool>(block->IsActive());
		writer->Data<UINT>(block->GetBlockInstanceID());
		writer->Data<Vector3>(block->GetLocalPosition());
	}
	delete writer;
}

void SubChunk::Load()
{
	string path = "Resources/Transforms/Map" + to_string(parentIndex) + to_string(index) + ".srt";

	BinaryReader* reader = new BinaryReader(path);

	int blockCount = reader->Data<int>();
	blocks.clear();
	
	blocks.reserve(blockCount);

	for (int i = 0; i < blockCount; i++)
	{
		UINT key = reader->Data<UINT>();
		bool isActive = reader->Data<bool>();
		UINT instanceID = reader->Data<UINT>();
		Vector3 position = reader->Data<Vector3>(); 
		
		UINT64 blockID = GameMath::GenerateBlockID(position);
	
		Block* newBlock = new Block(key);
		newBlock->SetActive(isActive);
		newBlock->SetLocalPosition(position);
		newBlock->SetBlockInstanceID(instanceID);
		newBlock->UpdateWorld();

		blocks[blockID] = newBlock;
	}

	delete reader;
}