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
	enum ItemType
	{
		BLOCK, CRAFTBLOCK, EMPTYBLOCK
	};
public:
	Block() {};
	Block(UINT key);
	virtual ~Block();

public:
	void Update();
	void Render();

	void EnableCollider();

	UINT GetItemKey() const { return itemData.key; }
	
	void SetItemKey(UINT key) { this->itemData.key = key; }

	virtual void Damage();
	
	bool HasCollider() { return hasCollider; }

	BoxCollider* GetCollider() { return collider; }

	int GetHp() { return curHp; }
	//void SetParentIndex(UINT64 parentIndex) { this->parentIndex = parentIndex; }
	//UINT64 GetParentIndex() { return parentIndex; }

	void CheckPlayerCollision();
	bool GetBlockType() { return blockType; }
	bool IsMining() { return isMining; }
	bool IsNormal() { return isNormal; }
	
	bool IsOcclusion() { return isOcclusion; }
	void SetOcclusion(bool isOcclusion) {this->isOcclusion= isOcclusion;}

	UINT GetBlockInstanceID() { return blockInstanceID; }
	void SetBlockInstanceID(UINT id) { this->blockInstanceID = id; }

	UVInfo GetUVInfo() { return uvInfo; }

private:
	void SetBlockUV();

protected:
	int curHp = 0;
	UINT64 parentIndex = 0;
	
	bool isMining = false;
	bool isOcclusion = false;

	bool isNormal = true;
	ItemType blockType = EMPTYBLOCK;
	ItemData itemData;

private:
	bool hasCollider = false;
	UINT blockInstanceID;
	UVInfo uvInfo;
	BoxCollider* collider = nullptr;
};