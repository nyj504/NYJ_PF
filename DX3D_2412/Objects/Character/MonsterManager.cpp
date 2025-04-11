#include "Framework.h"

MonsterManager::MonsterManager()
{
	monsters.reserve(MONSTER_COUNT * 2);

	for (int i = 0; i < MONSTER_COUNT; i++)
	{
		Animal* animal = new Animal("chicken");
		animal->SetActive(false);
		monsters.push_back(animal);

		Monster* monster = new Monster("Zombie");
		monster->SetActive(false);
		monsters.push_back(monster);
	}

	particle = new ParticleSystem("Resources/Textures/Particle/character_dead.fx");

	EventManager::Get()->AddEvent("ExcuteDie", [this]() 
		{ this->ExcuteDie(); });
	EventManager::Get()->AddEvent("ExcuteDamaged", [this]()
		{ this->ExcuteDamaged(); });
}

MonsterManager::~MonsterManager()
{
	for (Character* monster : monsters)
	{
		delete monster;
	}

	delete particle;
}

void MonsterManager::Update()
{
	spawnTimer += DELTA;

	if (spawnTimer >= SPAWN_INTERVAL)
	{
		spawnTimer -= SPAWN_INTERVAL;
		Spawn();
	}

	particle->Update();

	for (Character* monster : monsters)
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

	for (Character* monster : monsters)
	{
		if (monster->IsActive())
			monster->Render();
	}
}

void MonsterManager::GetDamaged()
{
	Ray ray = CAM->ScreenPointToRay(mousePos);
	RaycastHit hit;

	for (Character* monster : monsters)
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
	bool isNight = Sky::isNight;

	Vector3 playerPos = PLAYER->GetLocalPosition();
	Vector3 offset = { 4.0f, 0.0f, 4.0f };

	for (Character* monster : monsters)
	{
		if (!monster->IsActive() && playerPos.y > 0)
		{
			if (monster->GetTag() == "Animal" && !isNight)
			{
				monster->Spawn(playerPos + offset);
				return;
			}

			if (monster->GetTag() == "Monster" && isNight)
			{
				monster->Spawn(playerPos + offset);
				return;
			}
		}
	}
}

void MonsterManager::ExcuteDie()
{
	CharacterData data = curMonster->GetCharacterData();
	ItemManager::Get()->DropItems(data.dropItemKey, deadPosition, data.dropItemAmount);
	curMonster->SetActive(false);
	particle->Play(deadPosition);
	curMonster = nullptr;
}

void MonsterManager::ExcuteDamaged()
{
	attackMonster->ExcuteAttack();
}

vector<Character*> MonsterManager::GetActiveMonsters()
{
	vector<Character*> activeMonsters;

	for (Character* monster : monsters)
	{
		if (monster->IsActive())
			activeMonsters.push_back(monster);
	}
	return activeMonsters;
}
