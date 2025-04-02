#pragma once

struct UVInfo
{
	Vector2 uvStart, uvEnd;
};

struct InstanceData
{
	Matrix transform = XMMatrixIdentity();
	Vector2 curFrame;
	Vector2 maxFrame;

	UINT index;
	bool isActive;
};

class Block : public Transform
{
private:
	enum BlockType
	{
		BLOCK, CRAFTBLOCK
	};
public:
	Block() {};
	Block(UINT key);
	virtual ~Block();

public:
	void Render();
	void EnableCollider();

	UINT GetItemKey() const { return itemData.key; }
	void SetItemKey(UINT key) { this->itemData.key = key; }

	bool HasCollider() { return hasCollider; }
	BoxCollider* GetCollider() { return collider; }

	string GetParticlePath() { return itemData.particle; }

	bool GetBlockType() { return blockType; }
	WeaponType GetBlockWeakType() { return itemData.weakType; }
	
	bool IsNormal() { return isNormal; }
	
	bool IsOcclusion() { return isOcclusion; }
	void SetOcclusion(bool isOcclusion) {this->isOcclusion= isOcclusion;}

	UINT GetBlockInstanceID() { return blockInstanceID; }
	void SetBlockInstanceID(UINT id) { this->blockInstanceID = id; }

	UVInfo GetUVInfo() { return uvInfo; }
	void Mining();

private:
	void SetBlockUV();
	

private:
	UINT key;
	UINT64 parentIndex = 0;

	bool isOcclusion = false;

	bool isNormal = true;
	BlockType blockType = BLOCK;
	ItemData itemData;

	bool hasCollider = false;
	UINT blockInstanceID;
	UVInfo uvInfo;
	BoxCollider* collider = nullptr;
};