#include "Framework.h"

Monster::Monster(string name) : Character(name)
{
	modelAnimator = new ModelAnimator(name);
	modelAnimator->Load();

	modelAnimator->ReadClip("Zombie_Idle"); //0
	modelAnimator->ReadClip("Zombie_Walk"); //1
	modelAnimator->ReadClip("Zombie_Bite"); //2
	modelAnimator->ReadClip("Zombie_Dying"); //3
	modelAnimator->GetClip(2)->SetEvent([]() { EventManager::Get()->ExcuteEvent("ExcuteDamaged"); }, 0.40f);
	modelAnimator->GetClip(3)->SetEvent([]() { EventManager::Get()->ExcuteEvent("ExcuteDie"); }, 0.85f);
	modelAnimator->CreateTexture();
	modelAnimator->SetParent(this);

	collider = new BoxCollider();
	collider->SetTag("ZombieCollider");
	collider->SetParent(this);
	collider->Load();
}

Monster::~Monster()
{
	
}

void Monster::Update()
{
	sayingTimer += DELTA;
	if (sayingTimer >= SAYING_INTERVAL && monsterState != DIE)
	{
		sayingTimer -= SAYING_INTERVAL;

		int randNum = GameMath::Random(1, 4);
		Audio::Get()->Play("Zombie_say" + to_string(randNum), 0.3f);
	}

	Character::Update();
	UpdateWorld();

	TargetInRange();
	
	switch (monsterState)
	{
	case Monster::IDLE:
	{
		velocity.x = 0;
		velocity.z = 0;
	}
		break;
	case Monster::MOVE:
	{
		this->LookAt(PLAYER->GetLocalPosition());

		Vector3 dir = PLAYER->GetGlobalPosition() - this->GetGlobalPosition();
		dir.y = 0;
		dir.Normalize();

		velocity.x = dir.x;
		velocity.z = dir.z;
	}
		break;
	case Monster::ATTACK:
		velocity.x = 0;
		velocity.z = 0;
		break;
	case Monster::DIE:
		velocity = 0;
		break;
	}
	Move();
}

void Monster::Render()
{
	Character::Render();
	
}

void Monster::Move()
{
	Character::Move();
}

void Monster::Damaged(float damage, Character* target)
{
	Character::Damaged(damage, target);

	int hitSoundNum = GameMath::Random(1, 3);

	Audio::Get()->Play("Zombie_hurt" + to_string(hitSoundNum));

	if (curHp <= 0)
	{
		Audio::Get()->Play("Zombie_death");
		SetMonsterState(DIE);
	}
}

void Monster::SetMonsterState(MonsterState state)
{
	if (state == monsterState) return;

	monsterState = state; 

	if (modelAnimator)
	{
		int clipNum = (int)monsterState;
		modelAnimator->PlayClip(clipNum);
	}
	if (state == ATTACK)
	{
		MonsterManager::Get()->SetAttackingMonster(this);
	}
}

void Monster::ExcuteAttack()
{
	PLAYER->Damaged(characterData.atk, this);
	
	if (PLAYER->GetHp() <= 0)
	{
		PLAYER->SetPlayerState(PlayerState::DYING);
	}
	else
	{
		UIManager::Get()->GetPlayerHUDBar()->UpdateHpBar(PLAYER->GetHp());
	}
}

void Monster::TargetInRange()
{
	if (monsterState == DIE) return;
	
	float distance = Vector3::Distance(this->GetLocalPosition(), PLAYER->GetLocalPosition());
	if (distance <= characterData.range)
	{
		if (distance <= ATK_RANGE && monsterState)
		{
			SetMonsterState(ATTACK);
		}
		else
		{
			SetMonsterState(MOVE);
		}
	}
	else
	{
		SetMonsterState(IDLE);
	}
}

void Monster::Spawn(Vector3 pos)
{
	this->SetLocalPosition(pos);
	isAlive = true;
	isActive = true;
	curHp = characterData.maxHp;
	SetMonsterState(IDLE);
	UpdateWorld();
}
