#include "Framework.h"

Animal::Animal(string name) : Character(name)
{
	tag = "Animal";

    model = new Model(name);
    model->Load();
    model->SetParent(this);

    collider = new BoxCollider();
    collider->SetTag("chickenCollider");
    collider->Load();
	collider->SetParent(this);
}

Animal::~Animal()
{
}

void Animal::Update()
{
	sayingTimer += DELTA;
	
	if (sayingTimer >= SAYING_INTERVAL && animalState != DIE)
	{
		float distance = Vector3::Distance(PLAYER->GetLocalPosition(), this->GetLocalPosition());
		sayingTimer -= SAYING_INTERVAL;

		int randNum = GameMath::Random(1, 4);

		if(distance <= ACTIVATE_HOWLING)
			Audio::Get()->Play("Chicken_say" + to_string(randNum));
	}

	Character::Update();

	UpdateWorld();

	TargetOutRange();

	MoveSideways();

	switch (animalState)
	{
	case Animal::IDLE:
	{
		velocity.x = 0;
		velocity.z = 0;
	}
	break;
	case Animal::MOVE_AROUND:
	{
		idleWanderTimer += DELTA;
	}
	break;
	case Animal::RUN_AWAY:
	{
		this->EscapeFrom(PLAYER->GetLocalPosition());

		Vector3 dir = this->GetGlobalPosition() - PLAYER->GetGlobalPosition();
		dir.y = 0;
		dir.Normalize();

		velocity.x = dir.x;
		velocity.z = dir.z;
	}
	break;
	case Animal::DIE:
	{
		velocity = 0;
		deadTimer += DELTA;
		SetLocalRotation(0.0f, 0.0f, 90.0f);

		if (deadTimer >= ACTIVATE_PARTICLE && isAlive)
		{
			isAlive = false;
			EventManager::Get()->ExcuteEvent("ExcuteDie");
			Audio::Get()->Play("Chicken_hurt2", this->GetLocalPosition());
		}
	}
	break;
	}
	Move();
}

void Animal::Render()
{
	Character::Render();
}

void Animal::Move()
{
	if (animalState == DIE) return;

	Character::Move();
}

void Animal::Damaged(float damage, Character* target)
{
	int randNum = GameMath::Random(1, 3);
	Audio::Get()->Play("Chicken_hurt" + to_string(randNum), this->GetLocalPosition());

	Character::Damaged(damage, target);

	if (curHp <= 0)
	{
		SetAnimalState(DIE);
		return;
	}

	SetAnimalState(RUN_AWAY);
}

void Animal::TargetOutRange()
{
	if (animalState == DIE) return;

	float distance = Vector3::Distance(this->GetLocalPosition(), PLAYER->GetLocalPosition());

	if (distance >= characterData.range / 2)
	{
		SetAnimalState(MOVE_AROUND);
	}
}

void Animal::MoveSideways()
{
	if (idleWanderTimer >= WANDER_DELAY)
	{
		idleWanderTimer -= WANDER_DELAY;

		float offsetX = GameMath::Random(-3.0f, 3.0f);
		float offsetZ = GameMath::Random(-3.0f, 3.0f);

		Vector3 idleTargetPos = idlePosition + Vector3(offsetX, 0.0f, offsetZ);
		idleWanderTimer = 0.0f;

		LookAt(idleTargetPos);

		Vector3 dir = idleTargetPos - GetLocalPosition();
		dir.y = 0.0f;

		dir.Normalize();
		velocity.x = dir.x;
		velocity.z = dir.z;
	}
}

void Animal::Spawn(Vector3 pos)
{
	this->SetLocalPosition(pos);
	isAlive = true;
	isActive = true;
	curHp = characterData.maxHp;
	SetAnimalState(IDLE);
	UpdateWorld();
}
