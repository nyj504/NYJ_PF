#include "Framework.h"

InventoryCraftingSlot::InventoryCraftingSlot() : CraftBox(L"Resources/Textures/GUI/inventoryTopSlot.png")
{
    tag = "InventoryGUI";

	CreateSlot();

	craftingRecipes = DataManager::Get()->GetCraftingRecipes();
}

InventoryCraftingSlot::~InventoryCraftingSlot()
{
}

void InventoryCraftingSlot::Update()
{
	CraftBox::Update();
    for (CraftSlot* equipSlot : equipSlots)
        equipSlot->Update();
}

void InventoryCraftingSlot::PostRender()
{
    CraftBox::PostRender();
    for (CraftSlot* equipSlot : equipSlots)
        equipSlot->Render();
}

void InventoryCraftingSlot::EquipArmor()
{
    unordered_map<AmoType, UINT> equips;

    for (CraftSlot* equipSlot : equipSlots)
    {
        UINT equipKey = equipSlot->GetKey();
    
        if (equipSlot->GetTag() == "HelmetSlot")
            equips[AmoType::HELMET] = equipKey;
        else if (equipSlot->GetTag() == "ChestPlateSlot")
            equips[AmoType::CHESTPLATE] = equipKey;
        else if (equipSlot->GetTag() == "LeggingsSlot")
            equips[AmoType::LEGGINGS] = equipKey;
        else if (equipSlot->GetTag() == "BootsSlot")
            equips[AmoType::BOOTS] = equipKey;
    }

    if (!equips.empty())
        EquipManager::Get()->EquipTotalArmor(equips);
}

void InventoryCraftingSlot::CreateSlot()
{
	Vector3 startPos = { CENTER.x + 36, CENTER.y + 197};
	Vector2 interval = { 36, 36 };
	
	craftSlots.resize(MAX_SLOTSIZE);
    equipSlots.resize(MAX_EQUIPSIZE);

	int row = MAX_SLOTSIZE / INVEN_COL;
	int col = MAX_SLOTSIZE % INVEN_COL;

	for (int i = 0; i < MAX_SLOTSIZE - 1; i++)
	{
		CraftSlot* craftSlot = new CraftSlot(this);

		int row = i / INVEN_COL;
		int col = i % INVEN_COL;

		Vector3 pos = { startPos.x + col * interval.x, startPos.y - row * interval.y};

		craftSlot->SetLocalPosition(pos);
		craftSlot->UpdateWorld();

		craftSlots[i] = craftSlot;
	}

	CraftSlot* slot = new CraftSlot(this);
	slot->SetLocalPosition(startPos.x + 112 , startPos.y - 20, 0);
	slot->UpdateWorld();
	slot->SetTag("ResultSlot");

	craftSlots[MAX_SLOTSIZE - 1] = slot;

    for (int i = 0; i < MAX_EQUIPSIZE; i++)
    {
        Vector3 startPos = { CENTER.x - 144, CENTER.y + 217 };
        CraftSlot* slot = new CraftSlot(this);

        if(i == 0)
            slot->SetTag("HelmetSlot");
        if(i == 1)
            slot->SetTag("ChestPlateSlot");
        if(i == 2)
            slot->SetTag("LeggingsSlot");
        if(i == 3)
            slot->SetTag("BootsSlot");

        slot->SetLocalPosition(startPos.x, startPos.y - (interval.y * i), startPos.z);
        slot->UpdateWorld();

        equipSlots[i] = slot;
    }
}

void InventoryCraftingSlot::CraftItem()
{
    string serialKey;
  
    int minCount = 0;
    bool isChanged = false;


    for (int i = 0; i < craftSlots.size() - 1; i++)
    {
        UINT itemKey = craftSlots[i]->GetKey();
        UINT itemCount = craftSlots[i]->GetCount();
        if (itemKey == 23)
            itemKey = 22;

        serialKey += to_string(itemKey);

        if (i < craftSlots.size() - 2)
        {
            serialKey += "_"; 
        }
        
        if (itemKey > 0 && itemCount > 0)
        {
            if (itemCount > minCount)
            {
                minCount = itemCount;
            }
        }
        craftSlots[i]->SetRest(true);
    }

	unordered_map<string, pair<UINT, UINT>>::iterator it = craftingRecipes.find(serialKey);
       
    if (craftSlots[MAX_SLOTSIZE - 1]->IsChanged())
    {
        for (int i = 0; i < MAX_SLOTSIZE - 1; i++)
        {
            if (craftSlots[i]->GetKey() != 0 && INVEN->IsExcuteCrafting())
            {
                craftSlots[i]->DecreaseItem(minCount);
            }
        }
        craftSlots[MAX_SLOTSIZE - 1]->SetRest(true);
        return;
    }

    if (it != craftingRecipes.end())
    {
        UINT resultKey = it->second.first;
        UINT resultCount = it->second.second;
        UINT finalResultCount = resultCount * minCount;

        craftSlots[MAX_SLOTSIZE - 1]->SetItem(resultKey, finalResultCount);
        craftSlots[MAX_SLOTSIZE - 1]->SetRest(true);
    }
    else
    {
        craftSlots[MAX_SLOTSIZE - 1]->SetItem(0, 0);
        craftSlots[MAX_SLOTSIZE - 1]->SetRest(true);
    }
}
