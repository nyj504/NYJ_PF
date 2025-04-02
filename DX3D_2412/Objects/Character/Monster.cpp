#include "Framework.h"

Monster::Monster(string name) : Character(name)
{
	if (name == "Zombie")
	{
		modelAnimator->PlayClip(2);
	}
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
		break;
	case Monster::INRANGE:
	{
		Vector3 dir = PLAYER->GetLocalPosition() - this->GetLocalPosition();
		dir.Normalize();
		velocity.x = dir.x;
		velocity.z = dir.z;
	}
		break;
	case Monster::ATTACK:
		break;
	case Monster::DIE:
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

void Monster::TargetInRange()
{
	if (Vector3::Distance(this->GetLocalPosition(), PLAYER->GetLocalPosition()) <= IN_RANGE)
	{
		SetMonsterState(INRANGE);
	}
}
