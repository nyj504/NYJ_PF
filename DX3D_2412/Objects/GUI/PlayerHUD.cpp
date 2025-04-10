#include "Framework.h"

PlayerHUD::PlayerHUD()
{
	CreateHUDSlot();
}

PlayerHUD::~PlayerHUD()
{
	delete hpBar;
	delete armorBar;
	delete hungerBar;
}

void PlayerHUD::Render()
{
	hpBar->Render();
	armorBar->Render();
	hungerBar->Render();
}

void PlayerHUD::UpdateArmorBar(int count)
{
	armorBar->UpdateBar("armor", count);
}

void PlayerHUD::UpdateHpBar(int amount)
{
	hpBar->UpdateBar("heart", amount);
}

void PlayerHUD::UpdateHungerBar(int amount)
{
	hungerBar->UpdateBar("hunger", amount);
}

void PlayerHUD::CreateHUDSlot()
{
	hpBar = new HUDBar();
	hpBar->Create("Resources/Textures/UI/heart_full.png", Vector3(CENTER.x - 260, CENTER.y - 280));
	hpBar->UpdateBar("heart", 20);

	armorBar = new HUDBar();
	armorBar->Create("Resources/Textures/UI/armor_empty.png", Vector3(CENTER.x - 260, CENTER.y - 256));
	armorBar->UpdateBar("armor", 0);

	hungerBar = new HUDBar();
	hungerBar->Create("Resources/Textures/UI/hunger_full.png", Vector3(CENTER.x + 86, CENTER.y - 280));
}
