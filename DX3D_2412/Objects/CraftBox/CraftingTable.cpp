#include "Framework.h"

CraftingTable::CraftingTable() : CraftBox(L"Resources/Textures/GUI/crafting_tableSlot.png")
{
    tag = "CraftingTable";
	CreateSlot();
}

CraftingTable::~CraftingTable()
{
}

void CraftingTable::CreateSlot()
{
	Vector3 startPos = { CENTER.x - 100 , CENTER.y + 199 , 0 };
	Vector2 interval = { 36, 36 };

	craftSlots.resize(MAX_SLOTSIZE);

	int row = MAX_SLOTSIZE / INVEN_COL;
	int col = MAX_SLOTSIZE % INVEN_COL;

	for (int i = 0; i < MAX_SLOTSIZE - 1; i++)
	{
		CraftSlot* craftSlot = new CraftSlot(this);

		int row = i / INVEN_COL;
		int col = i % INVEN_COL;

		Vector3 pos = { startPos.x + col * interval.x, startPos.y - row * interval.y, 0 };

		craftSlot->SetLocalPosition(pos);
		craftSlot->UpdateWorld();

		craftSlots[i] = craftSlot;
	}

	CraftSlot* craftSlot = new CraftSlot(this);
	craftSlot->SetLocalPosition(startPos.x + 188, startPos.y - 36, 0);
	craftSlot->SetLocalScale(1.5f, 1.5f, 1.5f);
	craftSlot->SetTag("ResultSlot");
	craftSlot->UpdateWorld();

	craftSlots[MAX_SLOTSIZE - 1] = craftSlot;
}


void CraftingTable::CraftItem()
{
    string serialKey;

    unordered_map<UINT, UINT> itemCounts;
    int minCount = INT_MAX;
    bool isChanged = false;

    for (int i = 0; i < craftSlots.size() - 1; i++)
    {
        UINT itemKey = craftSlots[i]->GetKey();
        UINT itemCount = craftSlots[i]->GetCount();
        if (itemCount <= 0)
            itemKey = 0;
        if (itemKey == 23)
            itemKey = 22;

        serialKey += to_string(itemKey);

        if (i < craftSlots.size() - 2)
        {
            serialKey += "_";
        }
        if (itemKey > 0 && itemCount > 0)
        {
            itemCounts[itemKey] = itemCount;
            if (itemCount < minCount)
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
        craftSlots[MAX_SLOTSIZE - 1]->SetChanged(false);
    }
    else
    {
        craftSlots[MAX_SLOTSIZE - 1]->SetItem(0, 0);
        craftSlots[MAX_SLOTSIZE - 1]->SetChanged(false);
    }
}

