#pragma once
class Furnace : public CraftBox
{
private:
	const int MAX_SLOTSIZE = 3;
	const float SMELTING_TIME = 2.0f;
public:
	Furnace();
	~Furnace();

	void Update() override;
	
	// CraftBox을(를) 통해 상속됨
	void CreateSlot() override;

	// CraftBox을(를) 통해 상속됨
	void CraftItem() override;
	void MeltingItem();

private:
	bool canSmelting = false;
	bool isFuel = false;

	float meltingTimer = 0.0f;
};