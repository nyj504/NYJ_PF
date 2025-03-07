#pragma once
class CraftingTable : public CraftBox
{
private:
	const int MAX_SLOTSIZE = 10;
	const int INVEN_COL = 3;

public:
	CraftingTable();
	~CraftingTable();

	//void Update();

	void CreateSlot() override;

	// CraftBox을(를) 통해 상속됨
	void CraftItem() override;

};