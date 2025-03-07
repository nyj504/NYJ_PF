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

	// CraftBox��(��) ���� ��ӵ�
	void CreateSlot() override;

	// CraftBox��(��) ���� ��ӵ�
	void CraftItem() override;
};