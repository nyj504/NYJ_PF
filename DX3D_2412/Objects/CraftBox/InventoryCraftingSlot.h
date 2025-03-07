#pragma once
class InventoryCraftingSlot : public CraftBox
{
private:
	const int MAX_SLOTSIZE = 5;
	const int INVEN_COL = 2;
public:
	InventoryCraftingSlot();
	~InventoryCraftingSlot();

	void Update();

	// CraftBox을(를) 통해 상속됨
	void CreateSlot() override;

	// CraftBox을(를) 통해 상속됨
	void CraftItem() override;
};