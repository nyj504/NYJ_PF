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

	//void BuildBlock();
	void MiningBlock();
	void InteractingBlock();

	void ActivateCollisionBlocks();
	void ActivateRenderingChunks();

	Block* GetSelectedBlock() { return selectedBlock; }
	void SetSelectedBlock(Block* block) { selectedBlock = block; }

	//void Save();  
	//void Load();
private:
	Vector3 lastPlayerPos; // 마지막 플레이어 위치
	float updateThreshold = 32.0f;

	Block* selectedBlock = nullptr;
	
	unordered_map<UINT64, MainChunk*>activeChunks;

	WorldGenerator* worldGenerator;

	Cube* block;

	//vector<InstanceData> instanceDatas;
	//VertexBuffer* instanceBuffer;
};