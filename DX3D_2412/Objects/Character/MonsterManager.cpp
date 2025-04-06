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

	particle = new ParticleSystem("Resources/Textures/Particle/character_dead.fx");

	EventManager::Get()->AddEvent("ExcuteDie", [this]() 
		{ this->ExcuteDie(); });
	EventManager::Get()->AddEvent("ExcuteDamaged", [this]()
		{ this->ExcuteDamaged(); });
}

MonsterManager::~MonsterManager()
{
	for (Monster* monster : monsters)
	{
		delete monster;
	}

	delete particle;
}

void MonsterManager::Update()
{
	particle->Update();

	for (Monster* monster : monsters)
	{
		if (monster->IsActive())
		{
			monster->Update();
		}
	}

	GetDamaged();
}

void MonsterManager::Render()
{
	particle->Render();

	for (Monster* monster : monsters)
	{
		if (monster->IsActive())
			monster->Render();
	}
}

void MonsterManager::GetDamaged()
{
	Ray ray = CAM->ScreenPointToRay(mousePos);
	RaycastHit hit;

	for (Monster* monster : monsters)
	{
		if (monster->IsActive())
		{
			if (monster->GetCollider()->IsRayCollision(ray, &hit))
			{
				if (KEY->Down(VK_LBUTTON) && hit.distance < 2.0f)
				{
					monster->Damaged(PLAYER->GetPlayerEquipInfo().weaponAtk, PLAYER);
					curMonster = monster;

					if (monster->GetHp() <= 0.0f)
					{
						deadPosition = monster->GetLocalPosition() - monster->GetCollider()->HalfSize();
					}
				}
			}
		}
	}
}

void MonsterManager::Spawn()
{
	for (Monster* monster : monsters)
	{
		if (!monster->IsActive())
		{
			monster->Spawn(Vector3(5, 4, 5));
			return;
		}
	}
}

void MonsterManager::ExcuteDie()
{
	CharacterData data = curMonster->GetCharacterData();
	ItemManager::Get()->DropItem(data.dropItemKey, deadPosition, data.dropItemAmount);
	curMonster->SetActive(false);
	particle->Play(deadPosition);
	curMonster = nullptr;
}

void MonsterManager::ExcuteDamaged()
{
	attackMonster->ExcuteAttack();
}

Monster* MonsterManager::GetMonsters()
{
	for (Monster* monster : monsters)
	{
		if (monster->IsActive())
		{
			return monster;
		}
		else
			return nullptr;
	}
}
