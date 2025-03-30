#include "Framework.h"
#include "CrackEffect.h"

CrackEffect::CrackEffect() : Cube(Vector3(1.01f, 1.01f, 1.01f)), isAdditive(false)
{
    material->SetShader(L"FX/Sprite.hlsl");
    material->SetDiffuseMap(L"Resources/Textures/FX/Cracked_Block.png");

    spriteBuffer = new SpriteBuffer;
    spriteBuffer->Get().maxFrame = { frameX, frameY };

    maxFrame.x = (int)frameX;
    maxFrame.y = (int)frameY;
}

CrackEffect::~CrackEffect()
{
    delete spriteBuffer;
}

void CrackEffect::Update()
{
    if (!isActive && !PLAYER->IsMining()) return;

    Cube::Update();

    UpdateFrame();
}

void CrackEffect::Render()
{
    if (!isActive && !PLAYER->IsMining()) return;

    spriteBuffer->SetPS(10);

    if (isAdditive)
        Environment::Get()->SetAdditive();
    else
        Environment::Get()->SetAlphaBlend(true);

    Cube::Render();

    Environment::Get()->SetAlphaBlend(false);
}

void CrackEffect::UpdateFrame()
{
    time += DELTA;

    if (time >= intervalSpeed)
    {
        time -= intervalSpeed;

        curFrame = ++curFrame % (maxFrame.x * maxFrame.y);

        if (curFrame / maxFrame.x == 1 && curFrame % maxFrame.x == 3)
        {
            ResetMining();
            BlockManager::Get()->CallDoneMining();
            return;
        }

        spriteBuffer->Get().curFrame.x = curFrame % maxFrame.x;
        spriteBuffer->Get().curFrame.y = curFrame / maxFrame.x; 
    }
}

void CrackEffect::SetMining(Block* block)
{
    PlayerEquipmentInfo playerEquipmentType = PLAYER->GetPlayerEquipInfo();

    if (block->GetBlockWeakType() == playerEquipmentType.type)
    {
        float intervalTime = 1 / playerEquipmentType.weaponAtk;
        intervalSpeed = FRAME_INTERVAL * intervalTime;
    }
    else
        intervalSpeed = FRAME_INTERVAL;
    
    SetLocalPosition(block->GetLocalPosition());
    UpdateWorld();

    SetActive(true);
}

void CrackEffect::ResetMining()
{
    curFrame = 0;
    spriteBuffer->Get().curFrame.x = 0;
    spriteBuffer->Get().curFrame.y = 0;
    SetActive(false);
}
