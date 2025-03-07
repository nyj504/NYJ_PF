#include "Framework.h"

CraftingUI::CraftingUI()
{
	craftBoxes["CraftingTable"] = new CraftingTable();
	craftBoxes["CraftInventory"] = new InventoryCraftingSlot();
	craftBoxes["Furnace"] = new Furnace();

	for (const pair<string,CraftBox*>& craftBox : craftBoxes)
	{
		craftBox.second->SetLocalPosition(CENTER.x, CENTER.y + 166, 0);
		craftBox.second->UpdateWorld();
		craftBox.second->SetActive(false);
	}
}

CraftingUI::~CraftingUI()
{
	for (const pair<string, CraftBox*>& craftBox : craftBoxes)
	{
		delete craftBox.second;
	}
}

void CraftingUI::Update()
{
	if (!isActive) return;
	currentTopSlot->Update();
}

void CraftingUI::PostRender()
{
	if (!isActive) return;
	currentTopSlot->PostRender();
}

void CraftingUI::SetTopSlot(string type)
{
	if (craftBoxes.find(type) == craftBoxes.end()) return; 

	if (currentTopSlot)
		currentTopSlot->SetActive(false);
	isActive = !isActive;

	currentTopSlot = craftBoxes[type];
	currentTopSlot->SetActive(isActive);
	INVEN->SetActive(isActive);
}
