#include "Framework.h"

MonsterManager::MonsterManager()
{
	monsters.reserve(MONSTER_COUNT * 2);

	for (int i = 0; i < MONSTER_COUNT; i++)
	{
		Monster* monster = new Monster("Zombie");
		monster->SetActive(false);
		monsters.push_back(monster);

		Monster* animal = new Animal("chicken");
		animal->SetActive(false);
		monsters.push_back(animal);
	}
}

MonsterManager::~MonsterManager()
{
	for (Monster* monster : monsters)
	{
		delete monster;
	}
}

void MonsterManager::Update()
{
	for (Monster* monster : monsters)
	{
		if (monster->IsActive())
			monster->Update();
	}
}

void MonsterManager::Render()
{
	for (Monster* monster : monsters)
	{
		if (monster->IsActive())
			monster->Render();
	}
}

void MonsterManager::Spawn()
{
	for (Monster* monster : monsters)
	{
		if (!monster->IsActive())
		{
			monster->SetActive(true);
			monster->SetLocalPosition(5, 4, 5);
			return;
		}
	}
}

Monster* MonsterManager::GetMonsters()
{
	for (Monster* monster : monsters)
	{
		if (monster->IsActive())
		{
			return monster;
		}
	}
}
