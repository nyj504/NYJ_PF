#include "Framework.h"

Monster::Monster() : Character("Zombie")
{
	modelAnimator->PlayClip(2);
}

Monster::~Monster()
{
}

void Monster::Update()
{
	Character::Update();
	UpdateWorld();
}

void Monster::Render()
{
	Character::Render();
}
