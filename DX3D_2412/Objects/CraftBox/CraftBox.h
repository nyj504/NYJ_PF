#pragma once
class CraftBox : public Quad
{
public:
	CraftBox(wstring path);
	~CraftBox();

	virtual void Update();
	virtual void PostRender();

	virtual void CreateSlot() = 0;

	virtual void AddItem(UINT key, UINT count) {};
	virtual void DecreaseItem(UINT key, UINT count) {};
	
	virtual void CraftItem() = 0;
protected:
	vector<CraftSlot*> craftSlots;

	unordered_map<string, pair<UINT, UINT>> craftingRecipes;
};