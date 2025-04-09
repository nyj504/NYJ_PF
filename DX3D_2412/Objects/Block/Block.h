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

	ItemData GetItemData() const { return itemData; }

	bool HasCollider() { return hasCollider; }
	BoxCollider* GetCollider() { return collider; }

	bool GetBlockType() { return blockType; }

	bool IsNormal() { return isNormal; }
	
	bool IsOcclusion() { return isOcclusion; }
	void SetOcclusion(bool isOcclusion) {this->isOcclusion= isOcclusion;}

	UINT GetBlockInstanceID() { return blockInstanceID; }
	void SetBlockInstanceID(UINT id) { this->blockInstanceID = id; }

	void SetBlockID(UINT64 id) {this->blockID = id;}
	UINT64 GetBlockID(UINT64 id) { return blockID;}

	void SetParentIndex(UINT parentIndex) { this->parentIndex = parentIndex; }
	UINT GetParentIndex() { return parentIndex; }
	UVInfo GetUVInfo() { return uvInfo; }

	void Mining();

private:
	void SetBlockUV();
	

private:
	UINT key;
	UINT parentIndex = 0;
	UINT64 blockID = 0;

	bool isOcclusion = false;

	bool isNormal = true;
	BlockType blockType = BLOCK;
	ItemData itemData;

	bool hasCollider = false;
	UINT blockInstanceID;
	UVInfo uvInfo;
	BoxCollider* collider = nullptr;
};