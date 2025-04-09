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
}


void CraftSlot::SetItem(UINT key, UINT count)
{
	InventorySlot::SetItem(key, count);
	
	if (tag == "HelmetSlot" || tag == "ChestPlateSlot" ||
		tag == "LeggingsSlot" || tag == "BootsSlot")
	{
		craftBox->EquipArmor();
		isChanged = false;
	}

	if (tag == "CraftSlot")
	{
		craftBox->CraftItem();
		isChanged = false;
	}
}
