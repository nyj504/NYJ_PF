#include "Framework.h"

SimpleBlock::SimpleBlock(UINT key) : Block(key)
{
    itemData = DataManager::Get()->GetItemData(key);

    if (itemData.canBuild)
    {
        curHp = itemData.hp;
    }

    SetParent(this);

    if (key == 35)
    {
        tag = "CraftingTable";
    }
    if (key == 37)
    {
        tag = "Furnace";
    }

    if (itemData.textureType == "UVMapping")
    {
        //ApplyObjectUVMapping();
    }

    UpdateWorld();
}

SimpleBlock::~SimpleBlock()
{
}
