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
	
	if (tag == "CraftSlot" || tag == "ResultSlot")
	{
		TriggerCraftItem();
	}
	else if (tag == "HelmetSlot" || tag == "ChestPlateSlot" || 
		tag == "LeggingsSlot" || tag == "BootsSlot" || slotKey != 0)
	{
		TriggerEquipItem();
	}
}

void CraftSlot::TriggerCraftItem()
{
	if (isChanged)
	{
		craftBox->CraftItem();
		isChanged = false;
	}
}

void CraftSlot::TriggerEquipItem()
{
	if (isChanged)
	{
		craftBox->EquipArmor();
		isChanged = false;
	}
}
