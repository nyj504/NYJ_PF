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

    switch (characterType)
    {
    case Character::CharacterType::STEVE:
        modelAnimator = new ModelAnimator(name);
        modelAnimator->Load();

        modelAnimator->ReadClip("Idle"); //0
        modelAnimator->ReadClip("Walk"); 
        modelAnimator->ReadClip("Run");
        modelAnimator->ReadClip("Mining");
        modelAnimator->ReadClip("Dying"); //1
        modelAnimator->ReadClip("Dance"); //5
        modelAnimator->CreateTexture();
        modelAnimator->SetParent(this);

        collider = new BoxCollider();
        collider->SetTag("PlayerCollider");
        collider->SetParent(this);
        collider->Load();
        break;
    case Character::CharacterType::ANIMAL:
        model = new Model(name);
        model->Load();
        model->SetParent(this);

        collider = new BoxCollider();
        collider->SetTag("chickenCollider");
        collider->Load();
        break;
    case Character::CharacterType::MONSTER:
        modelAnimator = new ModelAnimator(name);
        modelAnimator->Load();

        modelAnimator->ReadClip("Zombie_Idle"); 
        modelAnimator->ReadClip("Zombie_Walk");
        modelAnimator->ReadClip("Zombie_Bite"); 
        modelAnimator->ReadClip("Zombie_Dying"); 
        modelAnimator->CreateTexture();
        modelAnimator->SetParent(this);

        collider = new BoxCollider();
        collider->SetTag("ZombieCollider");
        collider->SetParent(this);
        collider->Load();
        break;
    default:
        break;
    }
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
