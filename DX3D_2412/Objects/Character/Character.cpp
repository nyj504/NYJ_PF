#include "Framework.h"

Character::Character(string name)
{
    if (name == "SteveRigged")
        characterType = CharacterType::STEVE;
    else if(name == "Zombie")
        characterType = CharacterType::MONSTER;
    else if (name == "chicken")
        characterType = CharacterType::ANIMAL;

    switch (characterType)
    {
    case Character::CharacterType::STEVE:
        modelAnimator = new ModelAnimator(name);
        modelAnimator->Load();

        modelAnimator->ReadClip("Idle"); //0
        modelAnimator->ReadClip("Dying"); //1
        modelAnimator->ReadClip("Jump"); //2
        modelAnimator->ReadClip("Mining"); //3
        modelAnimator->ReadClip("Run"); //4
        modelAnimator->ReadClip("Walk"); //5
        modelAnimator->ReadClip("Dance"); //6 
        modelAnimator->ReadClip("Mining"); //7
        modelAnimator->CreateTexture();
        modelAnimator->SetParent(this);

        collider = new BoxCollider();
        collider->SetTag("PlayerCollider");
        collider->SetParent(modelAnimator);
        collider->UpdateWorld();
        collider->Load();
        break;
    case Character::CharacterType::ANIMAL:
        model = new Model(name);
        model->Load();
        model->SetParent(this);

        collider = new BoxCollider();
        collider->SetTag("chickenCollider");
        collider->SetParent(model);
        collider->UpdateWorld();
        collider->Load();
        break;
    case Character::CharacterType::MONSTER:
        modelAnimator = new ModelAnimator(name);
        modelAnimator->Load();
        modelAnimator->SetParent(this);

        modelAnimator->ReadClip("Zombie_Bite"); //0
        modelAnimator->ReadClip("Zombie_Dying"); //1
        modelAnimator->ReadClip("Zombie_Walk"); //2
        modelAnimator->CreateTexture();

        collider = new BoxCollider();
        collider->SetTag("ZombieCollider");
        collider->SetParent(modelAnimator);
        collider->UpdateWorld();
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
