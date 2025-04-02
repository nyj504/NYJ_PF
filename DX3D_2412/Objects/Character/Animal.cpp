#include "Framework.h"

Animal::Animal(string name) : Monster(name)
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
