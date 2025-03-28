#include "Framework.h"

Furnace::Furnace() : CraftBox(L"Resources/Textures/GUI/blast_furnaceSlot.png")
{
	tag = "Furnace";
	CreateSlot();
}

Furnace::~Furnace()
{
}

void Furnace::CreateSlot()
{
	craftSlots.resize(MAX_SLOTSIZE);

	Vector2 interval = { 0, 88 };
	Vector2 startPos = { CENTER.x - 48 , CENTER.y + 199 };

	
	CraftSlot* craftSlot = new CraftSlot(this);
	craftSlot->SetLocalPosition(startPos.x, startPos.y, 0);
	craftSlots[0] = craftSlot; // ���

	craftSlot = new CraftSlot(this);
	craftSlot->SetLocalPosition(startPos.x, startPos.y - interval.y + 16, 0);
	craftSlot->UpdateWorld();

	craftSlots[1] = craftSlot; // ����

	craftSlot = new CraftSlot(this);
	craftSlot->SetLocalPosition(startPos.x + interval.x + 120, startPos.y - interval.y / 2 + 8, 0);
	craftSlot->SetLocalScale(1.5f, 1.5f, 1.5f);
	craftSlot->UpdateWorld();
	craftSlot->SetTag("ResultSlot");

	craftSlots[2] = craftSlot;
}


void Furnace::CraftItem()
{
    ItemData ingredientData;

    bool isChanged = false;
    bool canSmelting = false;
    bool isFuel = false;

    UINT ingredientKey = craftSlots[0]->GetKey();
    UINT ingredientCount = craftSlots[0]->GetCount();
    UINT fuelKey = craftSlots[1]->GetKey();
    UINT fuelCount = craftSlots[1]->GetCount();

    if (ingredientKey != 0)
    {
        ingredientData = DataManager::Get()->GetItemData(ingredientKey);
    }

    if (craftSlots[0]->IsChanged() || craftSlots[1]->IsChanged())
    {
        if (ingredientKey != 0 && ingredientData.isSmeltable) 
            canSmelting = true;

        if (fuelKey != 0)
        {
            ItemData data = DataManager::Get()->GetItemData(fuelKey);
            if (data.isFuel) 
                isFuel = true;
        }
    }
    
    if (isFuel && canSmelting)
    {
        UINT totalResultCount = ingredientData.craftAmount * ingredientCount;

        craftSlots[2]->SetItem(stoi(ingredientData.serialKey), totalResultCount);
        craftSlots[1]->DecreaseItem(fuelKey, 1);
        craftSlots[0]->DecreaseItem(ingredientKey, ingredientCount);
    }
}
