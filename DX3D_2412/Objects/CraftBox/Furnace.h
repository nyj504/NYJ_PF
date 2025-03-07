#pragma once
class Furnace : public CraftBox
{
private:
	const int MAX_SLOTSIZE = 3;
public:
	Furnace();
	~Furnace();

	
	// CraftBox을(를) 통해 상속됨
	void CreateSlot() override;

	// CraftBox을(를) 통해 상속됨
	void CraftItem() override;

private:
	bool canSmelting = false;
};