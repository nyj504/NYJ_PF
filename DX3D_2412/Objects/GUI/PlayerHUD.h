#pragma once
class PlayerHUD
{
public:
	PlayerHUD();
	~PlayerHUD();

	void Render();
	void UpdateArmorBar(int count);
	void UpdateHpBar(int amount);
	void UpdateHungerBar(int amount);

	void CreateHUDSlot();

private:
	HUDBar* hpBar;
	HUDBar* armorBar;
	HUDBar* hungerBar;
};