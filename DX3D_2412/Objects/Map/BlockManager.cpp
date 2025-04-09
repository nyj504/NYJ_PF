#include "Framework.h"

BlockManager::BlockManager()
{
	worldGenerator = new WorldGenerator();
	
	particle = new ParticleSystem("Resources/Textures/Particle/particle.fx");
	
	crackEffect = new CrackEffect();
	crackEffect->SetActive(false);

	lastPlayerPos = PLAYER->GetGlobalPosition();
	//Load();
}

BlockManager::~BlockManager()
{
	delete worldGenerator;
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
	UINT selectedSlotNum = QUICKSLOT->GetSelectedIndex();
	
	if (quickSlotData.first == 0) return;

	ItemData data = DataManager::Get()->GetItemData(quickSlotData.first);
	
	if (!data.canBuild) return;

	Ray ray = CAM->ScreenPointToRay(mousePos);

	float minDistance = FLT_MAX;
	RaycastHit hit;

	if (selectedBlock->GetCollider()->IsRayCollision(ray, &hit))
	{
		Audio::Get()->Play("build");
		Vector3 buildPosition = selectedBlock->GetLocalPosition() + hit.normal;
		worldGenerator->BuildBlock(buildPosition, quickSlotData.first);
		INVEN->ConsumeItem(selectedSlotNum);
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
			string blockTexturePath = selectedBlock->GetItemData().particle + ".png";
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
	int renderDistance = 6; 

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

void BlockManager::Save()
{
	worldGenerator->Save();
}

void BlockManager::Load()
{
	worldGenerator->Load();
	ActivateRenderingChunks();
}

void BlockManager::CreateWorld()
{
	worldGenerator->CreateWorld();
	ActivateRenderingChunks();
}


