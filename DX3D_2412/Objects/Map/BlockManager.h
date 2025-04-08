#pragma once
class BlockManager : public Singleton<BlockManager>
{
private:
	friend class Singleton;

	BlockManager();
	~BlockManager();
public:
	void Update();
	void Render();
	void PostRender();

	void BuildBlock();
	void MiningBlock();
	void InteractingBlock();

	void CallDoneMining();
	void CallStopMining();
	
	void ActivateRenderingChunks();

	Block* GetSelectedBlock() { return selectedBlock; }
	CrackEffect* GetCrackEffect() { return crackEffect; }

	void SetSelectedBlock(Block* block) { selectedBlock = block; }
	void Save();  
	void Load();
private:
	bool isParticlePlayed = false;

	Vector3 lastPlayerPos; // 마지막 플레이어 위치
	float updateThreshold = 32.0f;

	Block* selectedBlock = nullptr;
	
	unordered_map<UINT64, MainChunk*>activeChunks;

	WorldGenerator* worldGenerator;

	Cube* block;

	ParticleSystem* particle;
	CrackEffect* crackEffect;

};