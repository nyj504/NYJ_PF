#include "Framework.h"

Animal::Animal() : Character("chicken")
{
}

Animal::~Animal()
{
}

void Animal::Update()
{
	Character::Update();

	UpdateWorld();
}

void Animal::Render()
{
	Character::Render();
}
