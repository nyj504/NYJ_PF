#pragma once
class ItemManager : public Singleton<ItemManager>
{
private:
	const int MAX_DROPITEM_COUNT = 100;

private:
	friend class Singleton;

	ItemManager();
	~ItemManager();

public:
	void Update();
	void Render();

	void DropItems(UINT key, Vector3 pos, UINT amounts);

private:
	vector<DropQuad*>dropItems;
	vector<DropCube*>dropSingleBlocks;
	vector<DropCube*>dropMultiBlocks;
};