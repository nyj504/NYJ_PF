#include "Framework.h"

BlockManager::BlockManager()
{
	worldGenerator = new WorldGenerator();
	
	particle = new ParticleSystem("Resources/Textures/Particle/particle.fx");
	
	crackEffect = new CrackEffect();
	crackEffect->SetActive(false);

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

	delete particle;
	delete crackEffect;
}

void BlockManager::Update()
{
	Vector3 currentPlayerPos = PLAYER->GetGlobalPosition();
	float distanceMoved = Vector3::Distance(currentPlayerPos, lastPlayerPos);
	
	worldGenerator->Update();

	particle->Update();
	crackEffect->Update();

	if (distanceMoved >= updateThreshold)
	{
		lastPlayerPos = currentPlayerPos;
		ActivateRenderingChunks();
	}
}

void BlockManager::Render()
{
	worldGenerator->Render();

	particle->Render();
	crackEffect->Render();
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
	Ray ray = CAM->ScreenPointToRay(mousePos);
	RaycastHit hit;

	if (selectedBlock->GetCollider()->IsRayCollision(ray, &hit))
	{
		if (!isParticlePlayed) 
		{
			string particlePath = "Resources/Textures/FX/";
			string blockTexturePath = selectedBlock->GetParticlePath() + ".png";
			particle->GetMaterial()->SetDiffuseMap(Utility::ToWString(particlePath + blockTexturePath));

			particle->GetParticleData().minAccelation = hit.normal * 2;
			particle->GetParticleData().duration = 0.5f;
			particle->GetParticleData().count = 10;
			particle->GetParticleData().isLoop = true;

			particle->Play(hit.point);
			isParticlePlayed = true;
		}
	}
}

void BlockManager::InteractingBlock()
{
	if (selectedBlock->GetBlockType() == 0) return;
	UIManager::Get()->SetTopSlot(selectedBlock->GetTag());
}

void BlockManager::CallDoneMining()
{
	CallStopMining();

	particle->Play(selectedBlock->GetLocalPosition());

	selectedBlock->Mining();
	worldGenerator->MiningBlock(selectedBlock);

	selectedBlock = nullptr;
}

void BlockManager::CallStopMining()
{
	particle->GetParticleData().count = 20;
	particle->GetParticleData().isLoop = false;
	isParticlePlayed = false;
}

void BlockManager::ActivateRenderingChunks()
{
	int renderDistance = 4; 

	activeChunks.clear();

	vector<MainChunk*> surroundingChunks = worldGenerator->GetChunksInRange(renderDistance);

	for (MainChunk* chunk : surroundingChunks)
	{
		UINT64 chunkKey = GameMath::ChunkPosToKey(chunk->GetChunkPosition().x / CHUNK_WIDTH,
			chunk->GetChunkPosition().z / CHUNK_DEPTH);

		worldGenerator->SetInstanceData(chunk, false);

		activeChunks[chunkKey] = chunk;
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



