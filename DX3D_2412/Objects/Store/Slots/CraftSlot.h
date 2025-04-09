#pragma once
class CraftBox;

class CraftSlot : public InventorySlot
{
public: 
	CraftSlot(CraftBox* craftBox);

	void Update();

	void SetItem(UINT key, UINT count) override;
private:
	CraftBox* craftBox;
};