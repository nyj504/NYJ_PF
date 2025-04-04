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

	if (deadTimer >= 0.0f)
	{
		deadTimer -= DELTA;
	}
	if (deadTimer <= 0.2f && !isActiveParticle)
	{
		particle->Play(deadPosition);
		isActiveParticle = true;
	}
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
					monster->Damaged(PLAYER->GetPlayerEquipInfo().weaponAtk);
					
					if (monster->GetHp() <= 0.0f)
					{
						deadTimer = 1.0f;
						deadPosition = monster->GetLocalPosition();
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
