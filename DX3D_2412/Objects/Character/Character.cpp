#include "Framework.h"

Character::Character(string name)
{
    modelAnimator = new ModelAnimator("SteveRigged");
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

    collider = new BoxCollider();
    collider->SetTag("PlayerCollider");
    collider->SetParent(modelAnimator);
    collider->UpdateWorld();
    collider->Load();
}

Character::~Character()
{
    delete collider;
    delete modelAnimator;
}

void Character::Update()
{
    modelAnimator->Update();

    modelAnimator->UpdateWorld();
    collider->UpdateWorld();
}

void Character::Render()
{
    modelAnimator->Render();
    collider->Render();
}
