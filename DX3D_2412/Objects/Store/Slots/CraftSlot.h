#pragma once
class CraftBox;

class CraftSlot : public InventorySlot
{
public: 
	CraftSlot(CraftBox* craftBox);

	void Update();

	void TriggerCraftItem();
private:
	CraftBox* craftBox;
};