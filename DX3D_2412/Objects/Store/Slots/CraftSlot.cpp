#include "Framework.h"

CraftSlot::CraftSlot(CraftBox* craftBox)
	: craftBox(craftBox)
{
	tag = "CraftSlot";
	slotKey = 0;   
	itemCount = 0;
}

void CraftSlot::Update()
{
	InventorySlot::Update();
	
	TriggerCraftItem();
}

void CraftSlot::TriggerCraftItem()
{
	if (isChanged)
	{
		craftBox->CraftItem();
		isChanged = false;
	}
}
