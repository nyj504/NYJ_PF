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
    delete collider;
}

void Block::Update()
{
    Vector3 playerPos = PLAYER->GetGlobalPosition();
    Vector3 blockPos = this->GetGlobalPosition();

    float xDistance = abs(playerPos.x - blockPos.x);
    float yDistance = abs(playerPos.y - blockPos.y);
    float zDistance = abs(playerPos.z - blockPos.z);

    if (xDistance <= 3 && yDistance <= 3 && zDistance <= 3)
    {
        CheckPlayerCollision();
    }
}

void Block::Render()
{
    if(collider)
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
    Vector3 overlap;

    Vector3 maxPlayerPosition = PLAYER->GetGlobalPosition() + PLAYER->GetCollider()->HalfSize();
    Vector3 maxBoxPosition = this->GetGlobalPosition() + collider->HalfSize();

    Vector3 minPlayerPosition = PLAYER->GetGlobalPosition() - PLAYER->GetCollider()->HalfSize();
    Vector3 minBoxPosition = this->GetGlobalPosition() - collider->HalfSize();

    Vector3 playerPosition = PLAYER->GetGlobalPosition();
    Vector3 blockPosition = this->GetGlobalPosition();

    Ray ray(PLAYER->GetGlobalPosition(), Vector3::Down());
    RaycastHit hit;

    float maxHeight = 0.0f;
    
    if (collider->IsRayCollision(ray, &hit))
    {
        if (hit.distance <= 0.3f && PLAYER->IsMove())
        {
            PLAYER->SetFall();
        }
    }
    
    if (collider->IsBoxCollision(PLAYER->GetCollider(), &overlap))
    {
        if (minPlayerPosition.y < maxBoxPosition.y && maxPlayerPosition.y > minBoxPosition.y)
        {
            if (overlap.y < abs(overlap.x) && overlap.y < abs(overlap.z))
            {
                if (playerPosition.y > blockPosition.y)
                {
                    PLAYER->Translate(0, overlap.y, 0);
                    PLAYER->SetLand();
                }
                else
                {
                    PLAYER->Translate(0, -overlap.y, 0);
                }
            }
            else if (abs(overlap.x) < abs(overlap.z))
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
