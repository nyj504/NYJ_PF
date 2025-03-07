#include "Framework.h"

Block::Block(UINT key)
{
    itemData = DataManager::Get()->GetItemData(key);
    blockType = (itemData.itemType == "Block") ? BLOCK : CRAFTBLOCK;

    isNormal = itemData.textureType == "Normal" ? 1 : 0;

    if (itemData.canBuild)
    {
        curHp = itemData.hp;
    }

    //GetMaterial()->SetDiffuseMap(Utility::ToWString(path));
  
    if (key == 35) tag = "CraftingTable";
    if (key == 37) tag = "Furnace";

    SetBlockUV();
}

Block::~Block()
{
}

void Block::Update()
{
    //UpdateWorld();
    //CheckPlayerCollision();
}

void Block::Damage()
{
    int damage = PLAYER->GetDamage();
    curHp -= damage;

    if (curHp <= 0)
    {
        INVEN->AddItem(itemData.dropItemKey, itemData.dropsAmount);
        SetActive(false);
        isMining = true;
    }
}

void Block::CheckPlayerCollision()
{
    //Vector3 overlap;
    //
    //Vector3 maxPlayerPosition = PLAYER->GetLocalPosition() + PLAYER->GetCollider()->HalfSize();
    //Vector3 maxBoxPosition = this->GetLocalPosition() + HalfSize();
    //
    //Vector3 minPlayerPosition = PLAYER->GetLocalPosition() - PLAYER->GetCollider()->HalfSize();
    //Vector3 minBoxPosition = this->GetLocalPosition() - HalfSize();
    //
    //Vector3 playerPosition = PLAYER->GetLocalPosition();
    //Vector3 blockPosition = this->GetLocalPosition();
    //
    //Ray ray(PLAYER->GetLocalPosition(), Vector3::Down());
    //RaycastHit hit;
    //float maxHeight = 0.0f;
    //
    //if (IsRayCollision(ray, &hit))
    //{
    //    if (hit.point.y >= minPlayerPosition.y)
    //    {
    //        PLAYER->SetLand();
    //    }
    //    else
    //    {
    //        PLAYER->SetFall();
    //    }
    //}
    //
    //if (IsBoxCollision(PLAYER->GetCollider(), &overlap) && minPlayerPosition.y < maxBoxPosition.y && maxPlayerPosition.y > minBoxPosition.y)
    //{
    //    if (minPlayerPosition.y < maxBoxPosition.y && maxPlayerPosition.y > minBoxPosition.y)
    //    {
    //        if (abs(overlap.x) < abs(overlap.z))
    //        {
    //            if (playerPosition.x < blockPosition.x)
    //                PLAYER->Translate(-overlap.x, 0, 0);
    //            else
    //                PLAYER->Translate(overlap.x, 0, 0);
    //        }
    //        else
    //        {
    //            if (playerPosition.z < blockPosition.z)
    //                PLAYER->Translate(0, 0, -overlap.z);
    //            else
    //                PLAYER->Translate(0, 0, overlap.z);
    //        }
    //    }
    //}
}

void Block::SetBlockUV()
{
    int index = itemData.textureType == "Normal" ? 0 : 1;

    int atlasWidth;
    int atlasHeight;

    switch (index)
    {       
    case 0:
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
    case 1:
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
    }
    default:
        break;
    }
}
