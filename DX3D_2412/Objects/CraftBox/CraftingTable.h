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

	// CraftBox��(��) ���� ��ӵ�
	void CraftItem() override;

};