#pragma once
class Furnace : public CraftBox
{
private:
	const int MAX_SLOTSIZE = 3;
public:
	Furnace();
	~Furnace();

	
	// CraftBox��(��) ���� ��ӵ�
	void CreateSlot() override;

	// CraftBox��(��) ���� ��ӵ�
	void CraftItem() override;

private:
	bool canSmelting = false;
};