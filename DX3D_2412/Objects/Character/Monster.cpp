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

void Monster::Attack()
{
	Vector3 overlap;
	if (collider->IsBoxCollision(PLAYER->GetCollider(), &overlap))
	{
		if(overlap.x > 0.1f && overlap.z > 0.1f)
		velocity = 0;
		SetMonsterState(ATTACK);
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
	if (Vector3::Distance(this->GetLocalPosition(), PLAYER->GetLocalPosition()) <= IN_RANGE)
	{
		this->LookAt(PLAYER->GetLocalPosition());

		Vector3 dir = PLAYER->GetGlobalPosition() - this->GetGlobalPosition();
		dir.y = 0;
		dir.Normalize();

		velocity.x = dir.x;
		velocity.z = dir.z;

		SetMonsterState(INRANGE);
	}
}
