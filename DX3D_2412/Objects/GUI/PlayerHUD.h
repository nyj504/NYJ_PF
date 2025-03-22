#pragma once
class PlayerHUD
{
public:
	PlayerHUD();
	~PlayerHUD();

	void Update();
	void Render();

	void CreateHUDSlot();

private:
	vector<Quad*>hungerSlots;
	vector<Quad*>healthSlots;
	vector<Quad*>armorSlots;
};