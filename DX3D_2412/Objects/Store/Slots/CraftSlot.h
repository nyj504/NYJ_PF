#pragma once
class CraftBox;

class CraftSlot : public InventorySlot
{
public: 
	CraftSlot(CraftBox* craftBox);

	void Update();

	void TriggerCraftItem();
	void TriggerEquipItem();
private:
	CraftBox* craftBox;
};