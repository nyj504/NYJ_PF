#include "Framework.h"

Block::Block(UINT key) : key(key)
{
    if (key > 0)
    {
        itemData = DataManager::Get()->GetItemData(key);
        blockType = (itemData.itemType == "Block") ? BLOCK : CRAFTBLOCK;

        isNormal = itemData.textureType == "Normal" ? 1 : 0;

        if (itemData.canBuild)
        {
            curHp = itemData.hp;
        }

        if (key == 35) tag = "CraftingTable";
        if (key == 37) tag = "Furnace";

        SetBlockUV();
    }
}

Block::~Block()
{
    delete collider;
}

void Block::Update()
{
    if (!isActive || isOcclusion) return;

     CheckPlayerCollision();
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

void Block::Damage()
{
    int damage = PLAYER->GetDamage();
    curHp -= damage;

    if (curHp <= 0)
    {
        Vector3 offset = { 0.0f, 0.2f, 0.0f };
        ItemManager::Get()->DropItem(itemData.dropItemKey, GetLocalPosition() - offset, itemData.dropsAmount);
        isActive = false;
        isMining = true;
    }
}

void Block::CheckPlayerCollision()
{
    Vector3 overlap;
    
    Vector3 maxPlayerPosition = PLAYER->GetLocalPosition() + PLAYER->GetCollider()->HalfSize();
    Vector3 maxBoxPosition = this->GetLocalPosition() + collider->HalfSize();
    
    Vector3 minPlayerPosition = PLAYER->GetLocalPosition() - PLAYER->GetCollider()->HalfSize();
    Vector3 minBoxPosition = this->GetLocalPosition() - collider->HalfSize();
    
    Vector3 playerPosition = PLAYER->GetLocalPosition();
    Vector3 blockPosition = this->GetLocalPosition();

    Ray ray(PLAYER->GetLocalPosition(), Vector3::Down());
    RaycastHit hit;
    bool isGrounded = false;

    if (collider->IsRayCollision(ray, &hit) && hit.point.y >= minPlayerPosition.y)
    {
        PLAYER->Translate(0, hit.point.y - minPlayerPosition.y, 0);
        isGrounded = true;
    }

    if (isGrounded || (minPlayerPosition.y <= maxBoxPosition.y && maxPlayerPosition.y >= minBoxPosition.y))
    {
        PLAYER->SetLand();
        
        if (collider->IsBoxCollision(PLAYER->GetCollider(), &overlap))
        {
            if (minPlayerPosition.y < maxBoxPosition.y && maxPlayerPosition.y > minBoxPosition.y)
            {
                if (abs(overlap.x) < abs(overlap.z))
                {
                    if (playerPosition.x < blockPosition.x)
                        PLAYER->Translate(-overlap.x, 0, 0);
                    else
                        PLAYER->Translate(overlap.x, 0, 0);
                }
                else
                {
                    if (playerPosition.z < blockPosition.z)
                        PLAYER->Translate(0, 0, -overlap.z);
                    else
                        PLAYER->Translate(0, 0, overlap.z);
                }
            }
        }
    }
    else
    {
        PLAYER->SetFall();
    }
  
    //Vector3 overlap;
    //
    //Vector3 maxPlayerPosition = PLAYER->GetGlobalPosition() + PLAYER->GetCollider()->HalfSize();
    //Vector3 maxBoxPosition = this->GetGlobalPosition() + collider->HalfSize();
    //
    //Vector3 minPlayerPosition = PLAYER->GetGlobalPosition() - PLAYER->GetCollider()->HalfSize();
    //Vector3 minBoxPosition = this->GetGlobalPosition() - collider->HalfSize();
    //
    //Vector3 playerPosition = PLAYER->GetGlobalPosition();
    //Vector3 blockPosition = this->GetGlobalPosition();
    //
    //Ray ray(PLAYER->GetGlobalPosition(), Vector3::Down());
    //RaycastHit hit;
    //
    //float maxHeight = 0.0f;
    //
    //if (!collider->IsBoxCollision(PLAYER->GetCollider(), &overlap))
    //    return;
    //
    //float absOverlapX = abs(overlap.x);
    //float absOverlapZ = abs(overlap.z);
    //float absOverlapY = abs(overlap.y);
    //
    //if (minPlayerPosition.y < maxBoxPosition.y && maxPlayerPosition.y > minBoxPosition.y)
    //{
    //    if (absOverlapY < absOverlapX && absOverlapY < absOverlapZ)
    //    {
    //        if (playerPosition.y > blockPosition.y)
    //        {
    //            PLAYER->Translate(0, overlap.y, 0);
    //            PLAYER->SetLand();
    //        }
    //        else
    //        {
    //            PLAYER->Translate(0, -overlap.y, 0);
    //            PLAYER->SetFall();
    //        }
    //    }
    //    else if (absOverlapX < absOverlapZ)
    //    {
    //        PLAYER->Translate(playerPosition.x < blockPosition.x ? -overlap.x : overlap.x, 0, 0);
    //    }
    //    else
    //    {
    //        PLAYER->Translate(0, 0, playerPosition.z < blockPosition.z ? -overlap.z : overlap.z);
    //    }
    //}
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
