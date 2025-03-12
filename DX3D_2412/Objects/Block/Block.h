#pragma once

struct UVInfo
{
	Vector2 uvStart, uvEnd;
};


class Block : public Transform
{
private:
	enum ItemType
	{
		BLOCK, CRAFTBLOCK
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
	
	int GetHp() { return curHp; }
	void SetIndex(UINT index) { this->index = index; }
	UINT GetIndex() { return index; }

	void CheckPlayerCollision();
	bool GetBlockType() { return blockType; }
	bool IsMining() { return isMining; }
	bool IsNormal() { return isNormal; }

	UVInfo GetUVInfo() { return uvInfo; }

private:
	void SetBlockUV();

protected:
	int curHp = 0;
	UINT index = 0;
	bool isMining = false;

	bool isNormal = true;
	ItemType blockType = BLOCK;
	ItemData itemData;

private:
	UVInfo uvInfo;
	BoxCollider* collider = nullptr;
};