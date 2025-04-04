#include "Framework.h"

Monster::Monster(string name) : Character(name)
{
	
}

Monster::~Monster()
{
	
}

void Monster::Update()
{
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

void Monster::Damaged(float damage)
{
	curHp -= damage;

	if (curHp <= 0)
	{
		SetMonsterState(DIE);
	}
}

void Monster::SetMonsterState(MonsterState state)
{
	if (state == monsterState) return;

	monsterState = state; 
	int clipNum = (int)monsterState;

	modelAnimator->PlayClip(clipNum);
}

void Monster::TargetInRange()
{
	if (monsterState == DIE) return;
	Vector3 overlap;

	float distance = Vector3::Distance(this->GetLocalPosition(), PLAYER->GetLocalPosition());
	if (distance <= IN_RANGE)
	{
		if (distance <= ATK_RANGE)
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
