#include "Framework.h"

Character::Character(string name)
{
    if (name == "SteveRigged")
    {
        characterKey = 1001;
        characterType = CharacterType::STEVE;
    }
    else if (name == "Zombie")
    {
        characterKey = 1002;
        characterType = CharacterType::MONSTER;
    }
    else if (name == "chicken")
    {
        characterKey = 1003;
        characterType = CharacterType::ANIMAL;
    }

    characterData = DataManager::Get()->GetCharacterData(characterKey);
    curHp = characterData.maxHp;
}

Character::~Character()
{
    delete collider;
   
    if(modelAnimator)
        delete modelAnimator;
    if(model)
        delete model;
}

void Character::Update()
{
    DeactivateHitState();

    if (modelAnimator)
    {
        modelAnimator->Update();
    }
    if (model)
    {
        model->UpdateWorld();
    }

    collider->UpdateWorld();
    UpdateWorld();
}

void Character::Render()
{
    if (modelAnimator)
    {
        modelAnimator->Render();
    }
    if (model)
    {
        model->Render();
    }
    collider->Render();
}

void Character::Move()
{
    Translate(velocity * characterData.moveSpeed * DELTA);
}


void Character::ActivateHitState()
{
    hitTimer = 0.0f;
    isHitEffect = true;

    vector<Material*> materials;

    if (characterType == CharacterType::ANIMAL )
        materials = model->GetMaterials();
    else
        materials = modelAnimator->GetMaterials();
    
    for (Material* material : materials)
    {
        material->GetData()->diffuse = { 1.0f, 0.0f, 0.0f, 1.0f };
    }
}

void Character::DeactivateHitState()
{
    if (isHitEffect)
    {
        hitTimer += DELTA;

        if (hitTimer >= HIT_DURATION)
        {
            vector<Material*> materials;

            if (characterType == CharacterType::ANIMAL)
            {
                materials = model->GetMaterials();
            }
            else
            {
                materials = modelAnimator->GetMaterials();
            }

            for (Material* material : materials)
            {
                material->GetData()->diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
            }

            isHitEffect = false;
        }
    }
}

void Character::MoveSideways()
{
    if (idleWanderTimer >= WANDER_DELAY)
    {
        idleWanderTimer -= WANDER_DELAY;

        float offsetX = GameMath::Random(-3.0f, 3.0f);
        float offsetZ = GameMath::Random(-3.0f, 3.0f);

        Vector3 idleTargetPos = idlePosition + Vector3(offsetX, 0.0f, offsetZ);
        idleWanderTimer = 0.0f;

        LookAt(idleTargetPos);

        Vector3 dir = idleTargetPos - GetLocalPosition();
        dir.y = 0.0f;

        dir.Normalize();
        velocity.x = dir.x;
        velocity.z = dir.z;
    }
}

void Character::Damaged(float damage, Character* target)
{
    curHp -= damage;

    Vector3 dir = this->GetLocalPosition() - target->GetLocalPosition();
    dir.y = 0.0f;
    dir.Normalize();

    this->Translate(dir * KNOCKBACK_RANGE);

    ActivateHitState();
}
