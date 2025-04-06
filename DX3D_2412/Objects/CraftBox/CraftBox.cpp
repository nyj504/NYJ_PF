#include "Framework.h"

CraftBox::CraftBox(wstring path) : Quad(path)
{
	craftingRecipes = DataManager::Get()->GetCraftingRecipes();

	EventManager::Get()->AddEvent("ExcuteCrafting", [this]()
		{this->CraftItem(); });
}

CraftBox::~CraftBox() 
{
	for (CraftSlot* craftSlot : craftSlots)
		delete craftSlot;
}

void CraftBox::Update()
{
	for (CraftSlot* craftSlot : craftSlots)
	{
		craftSlot->Update();
	}
}

void CraftBox::PostRender()
{
	if (!isActive) return;

	Quad::Render();

	for (CraftSlot* craftSlot : craftSlots)
		craftSlot->Render();
}

