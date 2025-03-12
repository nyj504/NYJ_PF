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
}

void SubChunk::Render()
{
	
}

void SubChunk::GenerateTerrain(Vector3 pos, UINT heightMap[CHUNK_WIDTH + 1][CHUNK_DEPTH + 1])
{
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

					if (worldY < terrainHeight - 5 && worldY >= terrainHeight - 16) // Y=33~48 (¾èÀº ÁöÇÏÃþ)
					{
						if (randValue < 15) blockType = 7;  // ¼®Åº (15% È®·ü)
						else if (randValue < 5) blockType = 8; // ±¸¸®±¤¼® (5% È®·ü)
						else if (randValue < 2) blockType = 10; // ±Ý±¤¼® (2% È®·ü) 
					}
					else if (worldY < terrainHeight - 16 && worldY >= terrainHeight - 32) // Y=17~32 (Áß°£ ÁöÇÏÃþ)
					{
						if (randValue < 12) blockType = 9;  // Ã¶±¤¼® (12% È®·ü)
						else if (randValue < 5) blockType = 10;  // ±Ý±¤¼® (5% È®·ü) 
						else if (randValue < 4) blockType = 12; // Ã»±Ý¼®(¶óÇÇ½º ¶óÁÙ¸®) (4% È®·ü) 
						else if (randValue < 1) blockType = 11; // ´ÙÀÌ¾Æ¸óµå (1% È®·ü) 
					}
					else if (worldY < terrainHeight - 32 && worldY >= terrainHeight - 40) // Y=9~16 (±íÀº ÁöÇÏÃþ)
					{
						if (randValue < 14) blockType = 9;  // Ã¶±¤¼® (14% È®·ü)
						else if (randValue < 6) blockType = 10;  // ±Ý±¤¼® (6% È®·ü) 
						else if (randValue < 5) blockType = 12; // Ã»±Ý¼® (5% È®·ü) 
						else if (randValue < 3) blockType = 13; // ·¹µå½ºÅæ (3% È®·ü)
						else if (randValue < 2) blockType = 11; // ´ÙÀÌ¾Æ¸óµå (2% È®·ü) 
					}
					else if (worldY < terrainHeight - 40 && worldY >= terrainHeight - 48) // Y=1~8 (½ÉÃþ ÁöÇÏÃþ)
					{
						if (randValue < 7) blockType = 13;  // ·¹µå½ºÅæ (7% È®·ü)
						else if (randValue < 5) blockType = 12;  // Ã»±Ý¼® (5% È®·ü) 
						else if (randValue < 7) blockType = 10;  // ±Ý±¤¼® (7% È®·ü) 
						else if (randValue < 4) blockType = 11;  // ´ÙÀÌ¾Æ¸óµå (4% È®·ü) 
					}
				}


				UINT gridIndex = (x * SUBCHUNK_HEIGHT * CHUNK_DEPTH) + (y * CHUNK_DEPTH) + z;

				pair<unordered_map<UINT, Block>::iterator, bool> result = blocks.emplace(gridIndex, Block(blockType));

				if (result.second) 
				{
					Block& newBlock = result.first->second;
					newBlock.SetParent(this);
					newBlock.SetLocalPosition(pos.x + x, worldY, pos.z + z);
					newBlock.SetIndex(gridIndex);
					newBlock.UpdateWorld();
				}
			}
		}
	}
}


Block* SubChunk::GetBlock(int x, int y, int z)
{
	if (x < 0 || x >= CHUNK_WIDTH ||
		y < 0 || y >= SUBCHUNK_HEIGHT ||
		z < 0 || z >= CHUNK_DEPTH)
	{
		return nullptr;
	}

	UINT index = (x * SUBCHUNK_HEIGHT * CHUNK_DEPTH) + (y * CHUNK_DEPTH) + z;

	auto it = blocks.find(index);
	if (it == blocks.end()) return nullptr;

	return &(it->second); 
}

void SubChunk::FindSurroundedBlocks()
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
				Block* block = GetBlock(x, y, z);
				if (!block) continue;

				Vector3 blockWorldPos = block->GetGlobalPosition();

				if (GetBlock(x + 1, y, z) && GetBlock(x - 1, y, z) &&
					GetBlock(x, y + 1, z) && GetBlock(x, y - 1, z) &&
					GetBlock(x, y, z + 1) && GetBlock(x, y, z - 1))
				{
					continue;
				} // ¿ÀÅ¬·çÁ¯ 

				//if (abs(playerPos.x - blockWorldPos.x) < 16 &&
				//	abs(playerPos.z - blockWorldPos.z) < 16)
				
				if (block->IsNormal())
				{
					visibleBlocks.push_back(block);

					InstanceData visibleInstanceData;
					UVInfo uvInfo = block->GetUVInfo();

					visibleInstanceData.transform = XMMatrixTranslation(blockWorldPos.x, blockWorldPos.y, blockWorldPos.z);
					visibleInstanceData.transform = XMMatrixTranspose(visibleInstanceData.transform);

					visibleInstanceData.curFrame = uvInfo.uvStart;
					visibleInstanceData.maxFrame = uvInfo.uvEnd;

					visibleSingleInstanceDatas.push_back(visibleInstanceData);
				}
				else
				{
					visibleBlocks.push_back(block);

					InstanceData visibleInstanceData;
					UVInfo uvInfo = block->GetUVInfo();

					visibleInstanceData.transform = XMMatrixTranslation(blockWorldPos.x, blockWorldPos.y, blockWorldPos.z);
					visibleInstanceData.transform = XMMatrixTranspose(visibleInstanceData.transform);

					visibleInstanceData.curFrame = uvInfo.uvStart;
					visibleInstanceData.maxFrame = uvInfo.uvEnd;

					visibleMultiInstanceDatas.push_back(visibleInstanceData);
				}
			}
		}
	}
}

void SubChunk::CheckVisibleBlocks()
{
	FindSurroundedBlocks();
}

