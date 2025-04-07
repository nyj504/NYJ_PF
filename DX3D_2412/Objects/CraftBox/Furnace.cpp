#include "Framework.h"

Furnace::Furnace() : CraftBox(L"Resources/Textures/GUI/blast_furnaceSlot.png")
{
	tag = "Furnace";
	CreateSlot();
}

Furnace::~Furnace()
{
}

void Furnace::Update()
{
    if (canSmelting && isFuel)
    {
        meltingTimer += DELTA;

        if (!Audio::Get()->IsPlaySound("blastfurnace"))
        {
            Audio::Get()->Play("blastfurnace");
        }
    }
       
    CraftBox::Update();

    if (meltingTimer >= SMELTING_TIME)
    {
        meltingTimer -= SMELTING_TIME;
        MeltingItem();
    }
}

void Furnace::CreateSlot()
{
	craftSlots.resize(MAX_SLOTSIZE);

	Vector2 interval = { 0, 88 };
	Vector2 startPos = { CENTER.x - 48 , CENTER.y + 199 };

	
	CraftSlot* craftSlot = new CraftSlot(this);
	craftSlot->SetLocalPosition(startPos.x, startPos.y, 0);
	craftSlots[0] = craftSlot; // 재료

	craftSlot = new CraftSlot(this);
	craftSlot->SetLocalPosition(startPos.x, startPos.y - interval.y + 16, 0);
	craftSlot->UpdateWorld();

	craftSlots[1] = craftSlot; // 연료

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
}

void Furnace::MeltingItem()
{
    UINT ingredientKey = craftSlots[0]->GetKey();
    UINT ingredientCount = craftSlots[0]->GetCount();
    UINT fuelKey = craftSlots[1]->GetKey();
    UINT fuelCount = craftSlots[1]->GetCount();

    if (fuelCount <= 0)
    {
        isFuel = false;
        canSmelting = false;
        return;
    }

    ItemData ingredientData = DataManager::Get()->GetItemData(ingredientKey);

    UINT totalResultCount = ingredientData.craftAmount * ingredientCount;

    craftSlots[2]->SetItem(stoi(ingredientData.serialKey), totalResultCount);
    craftSlots[1]->DecreaseItem(1);
    craftSlots[0]->DecreaseItem(ingredientCount);
}
