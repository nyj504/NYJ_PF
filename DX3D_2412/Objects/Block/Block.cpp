#include "Framework.h"

Block::Block(UINT key) : key(key)
{
    if (key > 0)
    {
        itemData = DataManager::Get()->GetItemData(key);
        blockType = (itemData.itemType == "Block") ? BLOCK : CRAFTBLOCK;

        isNormal = itemData.textureType == "Normal" ? 1 : 0;

        if (key == 35) tag = "CraftingTable";
        if (key == 37) tag = "Furnace";

        SetBlockUV();
    }
}

Block::~Block()
{
    delete collider;
}

void Block::Render()
{
    if (!isActive || isOcclusion) return;

    collider->Render();
}

void Block::EnableCollider()
{
    if (!collider)
    {
        collider = new BoxCollider();
        collider->SetParent(this);
        collider->UpdateWorld();

        hasCollider = true;
    }
    else
    {
        collider->UpdateWorld();
    }
}

void Block::Mining()
{
    Vector3 offset = { 0.0f, 0.3f, 0.0f };
    ItemManager::Get()->DropItems(itemData.dropItemKey, GetLocalPosition() - offset, itemData.dropsAmount);
    isActive = false;
    isOcclusion = true;
}

void Block::SetBlockUV()
{
    int atlasWidth;
    int atlasHeight;

    switch (isNormal)
    {
    case 1:
    {
        atlasWidth = 8;
        atlasHeight = 8;

        float gridSize = 1.0f / atlasWidth;

        int startU = (itemData.startUV - 1) % atlasWidth;
        int startV = (itemData.startUV - 1) / atlasWidth;

        Vector2 uvStart = { startU * gridSize, startV * gridSize };

        uvInfo.uvStart = uvStart;
        uvInfo.uvEnd = { uvStart.x + gridSize, uvStart.y + gridSize };
        break;

    }
    case 0:
    {
        atlasWidth = 4;
        atlasHeight = 7;

        float gridUSize = 1.0f / atlasWidth;
        float gridVSize = 1.0f / atlasHeight;

        int startU = (itemData.startUV - 1) % atlasWidth;
        int startV = (itemData.startUV - 1) / atlasWidth;

        Vector2 uvStart = { startU * gridUSize, startV * gridVSize };

        uvInfo.uvStart = uvStart;
        uvInfo.uvEnd = { uvStart.x + (4 * gridUSize), uvStart.y + gridVSize };
        break;
    }
    }
}
